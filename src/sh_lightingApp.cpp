#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class sh_lightingApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void sh_lightingApp::setup()
{
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
}

CINDER_APP( sh_lightingApp, RendererGl )
