#include "thirdparty.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class sh_lightingApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	CameraPersp cam;
	gl::BatchRef        object;
	gl::GlslProgRef		glsl;
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
			normal = normalize(ciNormalMatrix * ciNormal);
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
				c += coef[i] * basis[0];
			oColor = vec4(c, 1);
		}
	)));

	auto sphere = geom::Sphere().subdivisions(128);
	object = gl::Batch::create(sphere, glsl);

	double coefarr[16][3] = { { 2.02137, 2.10014, 2.19224 },
	{ 0.728084, 0.659208, 0.618444 },
	{ -0.0278568, -0.0393272, -0.049202 },
	{ 0.0672803, 0.0409734, 0.0428926 },
	{ 0.00966223, 0.0187042, 0.0394783 },
	{ 0.119934, 0.160242, 0.184851 },
	{ -0.153318, -0.162547, -0.177194 },
	{ -0.0466776, -0.0387007, -0.0361776 },
	{ -0.145646, -0.163856, -0.193314 },
	{ 0.29249, 0.281979, 0.278564 },
	{ -0.0306449, -0.0334904, -0.0349825 },
	{ 0.168543, 0.156271, 0.159067 },
	{ -0.0284515, -0.0350536, -0.0377981 },
	{ 0.15902, 0.15559, 0.16417 },
	{ 0.0491475, 0.0416729, 0.0415318 },
	{ 0.12386, 0.12666, 0.128872 } };
	vec3 coefs[16];
	for (int i = 0; i < 16; i++)
		coefs[i] = { coefarr[i][0], coefarr[i][1], coefarr[i][2] };
	glsl->uniform("coef", coefs, 16);

	gl::enableDepthWrite();
	gl::enableDepthRead();

}

void sh_lightingApp::mouseDown( MouseEvent event )
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
}

CINDER_APP( sh_lightingApp, RendererGl )
