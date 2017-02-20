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
	void update() override;
	void draw() override;

	CameraPersp cam;
	gl::BatchRef        object;
	gl::GlslProgRef		glsl;

	int mousex, mousey;
};

void sh_lightingApp::setup()
{
	cam.lookAt({ 3, 4, 5 }, {}, { 0, 1, 0 });
	glsl = gl::GlslProg::create(gl::GlslProg::Format()
		.vertex(CI_GLSL(150,
		uniform mat3 ciNormalMatrix;
		uniform mat4	ciModelViewProjection;
		in vec4			ciPosition;
		in vec3	ciNormal;

		out vec3 normal;
		void main(void) {
			gl_Position = ciModelViewProjection * ciPosition;
			//normal = normalize(ciNormalMatrix * ciNormal);
			normal = normalize(ciNormal);
		}
		))
	.fragment(CI_GLSL(150,
		const float PI = 3.1415926535897932384626433832795;
		in vec3 normal;
		uniform vec3 coef[16];

		out vec4 oColor;

		void main(void) {			
			float basis[16];

			float x = normal.x;
			float y = normal.y;
			float z = normal.z;
			float x2 = x*x;
			float y2 = y*y;
			float z2 = z*z;

			basis[0] = 1.f / 2.f * sqrt(1.f / PI);
			basis[1] = sqrt(3.f / (4.f*PI))*y;
			basis[2] = sqrt(3.f / (4.f*PI))*z;
			basis[3] = sqrt(3.f / (4.f*PI))*x;
			basis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * y;
			basis[5] = 1.f / 2.f * sqrt(15.f / PI) * y * z;
			basis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x*x - y*y + 2 * z*z);
			basis[7] = 1.f / 2.f * sqrt(15.f / PI) * z * x;
			basis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x*x - y*y);
			basis[9] = 1.f / 4.f*sqrt(35.f / (2.f*PI))*(3 * x2 - y2)*y;
			basis[10] = 1.f / 2.f*sqrt(105.f / PI)*x*y*z;
			basis[11] = 1.f / 4.f*sqrt(21.f / (2.f*PI))*y*(4 * z2 - x2 - y2);
			basis[12] = 1.f / 4.f*sqrt(7.f / PI)*z*(2 * z2 - 3 * x2 - 3 * y2);
			basis[13] = 1.f / 4.f*sqrt(21.f / (2.f*PI))*x*(4 * z2 - x2 - y2);
			basis[14] = 1.f / 4.f*sqrt(105.f / PI)*(x2 - y2)*z;
			basis[15] = 1.f / 4.f*sqrt(35.f / (2 * PI))*(x2 - 3 * y2)*x;

			vec3 c;			
			for (int i = 0; i < 16; i++)
				c += coef[i] * basis[i];
			oColor = vec4(c, 1);
		}
	)));

	auto sphere = geom::Sphere().subdivisions(128);
	object = gl::Batch::create(sphere, glsl);

	double coefarr[16][3] = { { 0.58948, 0.58948, 0.58948 },
	{ 0.811326, 0.811326, 0.811326 },
	{ 9.74207e-009, 9.74207e-009, 9.74207e-009 },
	{ -1.26291e-009, -1.26291e-009, -1.26291e-009 },
	{ 1.65055e-009, 1.65055e-009, 1.65055e-009 },
	{ 1.36179e-008, 1.36179e-008, 1.36179e-008 },
	{ -0.302541, -0.302541, -0.302541 },
	{ 6.5717e-011, 6.5717e-011, 6.5717e-011 },
	{ -0.524082, -0.524082, -0.524082 },
	{ -0.145148, -0.145148, -0.145148 },
	{ 1.19815e-009, 1.19815e-009, 1.19815e-009 },
	{ -0.112434, -0.112434, -0.112434 },
	{ 3.37324e-008, 3.37324e-008, 3.37324e-008 },
	{ -2.60844e-010, -2.60844e-010, -2.60844e-010 },
	{ -2.301e-009, -2.301e-009, -2.301e-009 },
	{ -1.03469e-009, -1.03469e-009, -1.03469e-009 } };
	vec3 coefs[16];
	for (int i = 0; i < 16; i++)
		coefs[i] = { coefarr[i][0], coefarr[i][1], coefarr[i][2] };
	glsl->uniform("coef", coefs, 16);

	gl::enableDepthWrite();
	gl::enableDepthRead();

}

void sh_lightingApp::mouseDown( MouseEvent event )
{
	auto pos = event.getPos();
	mousex = pos.x;
	mousey = pos.y;
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
	//object->draw();	
	gl::drawCube({}, { 1, 1, 1 });
}

CINDER_APP( sh_lightingApp, RendererGl )
