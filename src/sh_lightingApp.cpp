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
	cam.lookAt({ 3, 4, 5 }, {}, { 0, 1, 0 });
	sh_lighting_glsl = gl::GlslProg::create(loadAsset("sh_lighting.vert"), loadAsset("sh_lighting.frag"));

	auto sphere = geom::Sphere().subdivisions(128);
	object = gl::Batch::create(sphere, sh_lighting_glsl);

	double coefarr[16][3] = { { 0.588963, 0.58948, 0.591273 },
	{ 3.18013e-009, 0.811326, 0.00318166 },
	{ 6.23121e-010, 9.74207e-009, 0.808141 },
	{ 0.808141, -1.26291e-009, -4.40256e-010 },
	{ 1.88451e-008, 1.65055e-009, -9.7385e-011 },
	{ 4.63115e-010, 1.36179e-008, 1.88451e-008 },
	{ -0.301385, -0.302541, 0.601604 },
	{ -4.63115e-010, 6.5717e-011, -6.95029e-010 },
	{ 0.522027, -0.524082, -0.00205411 },
	{ 2.65911e-008, -0.145148, -0.0005692 },
	{ -1.73055e-009, 1.19815e-009, 7.5527e-010 },
	{ -2.05366e-009, -0.112434, -0.000440898 },
	{ 5.93596e-010, 3.37324e-008, 0.182879 },
	{ -0.11199, -2.60844e-010, 1.11969e-009 },
	{ -3.62364e-009, -2.301e-009, -5.16782e-008 },
	{ 0.144577, -1.03469e-009, -7.58246e-010 }};
	vec3 coefs[16];
	for (int i = 0; i < 16; i++)
		coefs[i] = { coefarr[i][0], coefarr[i][1], coefarr[i][2] };
	sh_lighting_glsl->uniform("coef", coefs, 16);

	// environment map
	string img_files[6] = {"posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg"};
	for (int i = 0; i < 6; i++)
		envimgs[i] = loadImage(loadAsset(img_files[i]));
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
