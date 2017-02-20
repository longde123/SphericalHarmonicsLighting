#include "thirdparty.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class sh_lightingApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseDrag(MouseEvent event) override;
	void mouseMove(MouseEvent event) override;
	void mouseWheel(MouseEvent event) override; 
	void resize() override;
	void update() override;
	void draw() override;

	CameraPersp cam;
	gl::BatchRef        object;
	gl::GlslProgRef		sh_lighting_glsl;

	ImageSourceRef envimgs[6];
	gl::TextureCubeMapRef	cubemap;
	gl::BatchRef			skybox;

	int mousex, mousey;
};

void sh_lightingApp::setup()
{
	auto cmds = this->getCommandLineArgs();
	if (cmds.size() < 8)
		throw runtime_error("Usage: sh_lighting posx negx posy negy posz negz coefficients.txt");
	string img_files[6];
	for (int i = 0; i < 6; i++)
		img_files[i] = cmds[i + 1];
	string sh_coef_file = cmds[7];
	double coefarr[16][3];
	ifstream ifs(sh_coef_file);
	if (!ifs)
		throw runtime_error("open " + sh_coef_file + " failed");
	vec3 coefs[16];
	for (int i = 0; i < 16; i++){
		ifs >> coefs[i].r >> coefs[i].g >> coefs[i].b;
	}

	cam.lookAt({ 3, 4, 5 }, {}, { 0, 1, 0 });
	sh_lighting_glsl = gl::GlslProg::create(loadAsset("sh_lighting.vert"), loadAsset("sh_lighting.frag"));

	auto sphere = geom::Sphere().subdivisions(128);
	object = gl::Batch::create(sphere, sh_lighting_glsl);

	sh_lighting_glsl->uniform("coef", coefs, 16);

	// environment map
	for (int i = 0; i < 6; i++)
		envimgs[i] = loadImage(loadFile(img_files[i]));
	cubemap = gl::TextureCubeMap::create(envimgs, gl::TextureCubeMap::Format().mipmap());
	auto envMapGlsl = gl::GlslProg::create(loadAsset("env_map.vert"), loadAsset("env_map.frag"));
	auto skyBoxGlsl = gl::GlslProg::create(loadAsset("sky_box.vert"), loadAsset("sky_box.frag"));
	skybox = gl::Batch::create(geom::Cube(), skyBoxGlsl);
	skybox->getGlslProg()->uniform("uCubeMapTex", 0);


	gl::enableDepthWrite();
	gl::enableDepthRead();

}

void sh_lightingApp::resize()
{
	cam.setPerspective(60, getWindowAspectRatio(), 1, 1000);
}

void sh_lightingApp::mouseDown( MouseEvent event )
{
	auto pos = event.getPos();
	mousex = pos.x;
	mousey = pos.y;
}

void sh_lightingApp::mouseWheel(MouseEvent event)
{
	float dw = 1.f - event.getWheelIncrement()*0.1f;
	auto p = cam.getEyePoint();
	auto neweye = glm::scale(glm::vec3{ dw, dw, dw })*glm::vec4{ p,1 };
	cam.lookAt(vec3(neweye), { 0, 0, 0 }, cam.getWorldUp());
}

void sh_lightingApp::mouseDrag(MouseEvent event)
{
	auto pos = event.getPos();
	float dx = -(pos.x - mousex)*0.01f;
	float dy = -(pos.y - mousey)*0.01f;
	auto p = cam.getEyePoint();
	auto up = cam.getWorldUp();
	auto dir = cam.getViewDirection();
	auto right = normalize(cross(dir, up));
	auto hrot = glm::rotate(dx, up);
	auto vrot = glm::rotate(dy, right);
	auto neweye = hrot*vrot*vec4{ p, 1 };
	cam.lookAt(vec3(neweye), {0,0,0}, up);
	mousex = pos.x;
	mousey = pos.y;
}

void sh_lightingApp::mouseMove(MouseEvent event)
{	
}

void sh_lightingApp::update()
{
}

void sh_lightingApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::setMatrices(cam);
	object->draw();	
	
	cubemap->bind();
	// draw sky box
	gl::pushMatrices();
	const int SKY_BOX_SIZE = 500;
	gl::scale(SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE);
	skybox->draw();
	gl::popMatrices();
}

CINDER_APP( sh_lightingApp, RendererGl )
