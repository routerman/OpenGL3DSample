#include "Include.hpp"
#include "common.hpp"
#include "Game.hpp"

class Game;
class Fighter;


Game *game=new Game(1);

///////////////////////// ハンドラ START /////////////////////////
void display(void){
	game->display();
}

void timer(int dt){	
	//MVC model
	game->control();
	game->timer();
	glutPostRedisplay();//call display()
	glutTimerFunc(dt,timer,10);
}

void keydown(unsigned char key,int x,int y){
	game->keydown(key);
	//glutPostRedisplay();
}

void special_keydown(int key,int x,int y){
	game->keydown(key);
	//glutPostRedisplay();
}

void keyup(unsigned char key,int x,int y){	
	game->keyup(key);
	//glutPostRedisplay();
}

void special_keyup(int key,int x,int y){
	game->keyup(key);
	//glutPostRedisplay();
}
///////////////////////// ハンドラ END /////////////////////////

///////////////////////// MAIN START /////////////////////////
void SetLinearAttenuationSpotLight( void )
{
    GLfloat light_position[] = { 0.0, 0.0, 100.0, 1.0 };

    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat spot_dir[] = { 0.0f, 0.0f, -1.0f };

    glLightfv( GL_LIGHT0, GL_POSITION, light_position );

    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );

    glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f );
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0f / 200.0f );
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f );

    glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir );
    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 30.0f );
    glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f );

    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

}

int main(int argc, char** argv){
	glutInit(&argc, argv); 
	game->init();	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0); 
	glutCreateWindow("OpenAir");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1, VFAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	//SetLinearAttenuationSpotLight();
	//key handler
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(special_keydown);
	glutSpecialUpFunc(special_keyup);
	//time handler
	//glutIdleFunc(idle);
	glutTimerFunc(0,timer,10);
	glutDisplayFunc(display);
	glutMainLoop(); 
	return 0;
}