#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 move: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {
  char to; char to2[256];
  double x0;double y0;double z0;
  double x1;double y1;double z1;
  color c; c.red = 255; c.green = 255; c.blue = 255;
  FILE *f;
  char line[256];
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    //printf(":%s:\n",line);
    
    if (!strcmp(line,"line")){
      //printf("Enter 6 proper coordinates:\n");
      scanf("%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &x1, &y1, &z1);
      //printf("%lf %lf %lf %lf %lf %lf\n", x0, y0, z0, x1, y1, z1);
      add_edge(edges,x0,y0,z0,x1,y1,z1);
      //print_matrix(edges);
    }
    if (!strcmp(line,"ident")){
      ident(transform);
      //print_matrix(transform);
    }
    if (!strcmp(line,"scale")){
      struct matrix *tmp;
      scanf("%lf %lf %lf", &x0, &y0, &z0);
      //printf("%lf %lf %lf\n", x0, y0, z0);
      tmp = make_scale(x0,y0,z0);
      matrix_mult(tmp,transform);
      //print_matrix(transform);
      free_matrix(tmp);
    }
    if (!strcmp(line, "move")){
      struct matrix *tmp;
      scanf("%lf %lf %lf", &x0, &y0, &z0);
      //printf("%lf %lf %lf\n", x0, y0, z0);
      tmp = make_translate(x0,y0,z0);
      matrix_mult(tmp,transform);
      //print_matrix(transform);
      free_matrix(tmp);
    }
    if (!strcmp(line, "rotate")){
      struct matrix *tmp;
      scanf("%c %lf", &to, &y0);
      //printf("%c %lf", to, y0);
      
      if(to == 'x'){
	tmp = make_rotX(y0);
      }
      else if(to == 'y'){
	tmp = make_rotY(y0);
      }
      else{
	tmp = make_rotZ(y0);
      }
      
      matrix_mult(tmp,transform);
      //print_matrix(transform);
      free_matrix(tmp);
    }
    if(!strcmp(line,"apply")){
      matrix_mult(transform,edges);
      //print_matrix(edges);
    }
    if(!strcmp(line,"display")){
      clear_screen(s);
      draw_lines(edges,s,c);
      display(s);
    }
    if(!strcmp(line,"save")){
      scanf("%s", to2);
      //printf("%s\n",to2);
      save_extension(s, to2);
      printf("image created!\n");
    }    
    if (!strcmp(line,"quit")){
      return;
    }
  }
}
/*
The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
     quit: end parsing
*/
