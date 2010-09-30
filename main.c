#include <stdio.h>
#include <X11/Xlib.h>
#include <argp.h>
#include <math.h>
#include <unistd.h>


const char *argp_program_version = "SetMouseFrame 08.09.08";
const char *argp_program_bug_address = "<none>";

/* Program documentation */
static char args_doc[] =
	"x width y height";

/* A description of the arguments we accept */
static char doc[] =
	"\n  x        The x-coordinate of the frame on the screen"
	"\n  width    The frame's width"
	"\n  y        The y-coordinate of the frame on the screen"
	"\n  height   The frame's height";

/* the options we understand */
static struct argp_option options[] =
{
	{ "verbose",	'v',	0,	0,	"Produce verbose output" },
	{ "frequency",	'f',	"timespan(ms)",	0,	"Set the poll frequency in milliseconds" },
	{ "screen",	's',	"screen#",	0,	"Set the X-Server-Screen the frame should be" },
	{	0	}
};

/* used by main to communicat with parse_opt */
struct arguments
{
	char *args[4];
	int verbose;
	int frequency;
	int screen;
};

/* parse a single option */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (key)
	{
		case 'v':
			arguments->verbose = 1;
			break;

		case 'f':
			arguments->frequency = charp2int (arg);
			break;

		case 's':
			arguments->screen = charp2int (arg);
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num > 4) argp_usage(state);
			arguments->args[state->arg_num] = arg;
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 4) argp_usage(state);
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

/* argp parse */
static struct argp argp = { options, parse_opt, args_doc, doc };


/* char[] to int -function */
int charp2int (char* digits)
{
	int length;
	int i;
	int num;

	length = strlen(digits);
	num = 0;

	for (i=0; i<length; i++)
	{
		num = num + (digits[i] - '0') * pow(10,length-(i+1));
	}

	return num;
}


int main (int argc, char **argv)
{
	Display *display;
	Window root;
	
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	
	Bool query;

	Window root_return;
	Window child_return;
	unsigned int mask_return;
	
	int screen_num;
	unsigned int display_width;
	unsigned int display_height;

	int frame_x;
	int frame_y;
	int frame_width;
	int frame_height;
	
	Bool doFlush;	


	/* parameter stuff */
	struct arguments arguments;
	arguments.verbose = 0;
	arguments.frequency = 50;
	arguments.screen = 0;

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	frame_x = charp2int (arguments.args[0]);
	frame_width = charp2int (arguments.args[1]);
	frame_y = charp2int (arguments.args[2]);
	frame_height = charp2int (arguments.args[3]);
	
	
	display = XOpenDisplay (NULL);	
	root = RootWindow(display, arguments.screen);

		
	/* LOOP */
	do
	{
		XQueryPointer(display, root, &root_return, &child_return, &root_x, &root_y, &win_x, &win_y, &mask_return);

		doFlush = False;
		if (root_x < frame_x) { XWarpPointer (display, None, root, 0, 0, 0, 0, frame_x, root_y); doFlush = True; root_x = frame_x; }
		if (root_x > frame_x + frame_width) { XWarpPointer (display, None, root, 0, 0, 0, 0, frame_x + frame_width, root_y); doFlush = True; root_x = frame_x + frame_width; }
		if (root_y < frame_y) { XWarpPointer (display, None, root, 0, 0, 0, 0, root_x, frame_y); doFlush = True; root_y = frame_y; }
		if (root_y > frame_y + frame_height) { XWarpPointer (display, None, root, 0, 0, 0, 0, root_x, frame_y + frame_height); doFlush = True; root_y = frame_y + frame_height; }
	
		if (doFlush) XFlush (display);
		usleep (arguments.frequency * 1000);
	} while (True); 
	/*	
	printf ("Root X: %d\n", root_x);
	printf ("Root Y: %d\n", root_y);
	printf ("Win X: %d\n", win_x);
	printf ("Win Y: %d\n", win_y);

	screen_num = DefaultScreen (display);
	display_width = DisplayWidth (display, screen_num);
	display_height = DisplayHeight (display, screen_num);
	*/
  	/* Query the display structure for information */
	/*	
	screen_num = DefaultScreen(display);
	display_width = DisplayWidth(display, screen_num);
	display_height = DisplayHeight(display, screen_num);
	*/
	/* Print the resolution into the shell */
	/*
	printf("screen_num: %d\n", screen_num);
	printf("resolution width: %d pixels\n", display_width);
	printf("resolution height: %d pixels\n", display_height);
	*/

	/* Close the Window and exit application */
	XCloseDisplay(display);
	

	return 0;
}
