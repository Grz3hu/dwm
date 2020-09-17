#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const int gappx     = 5;                 /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 8;        /* horizontal padding for statusbar */
static const int vertpadbar         = 7;        /* vertical padding for statusbar */
/* static const char *fonts[]          = { "monospace:size=15" }; */
static const char *fonts[]            = { "mononoki-Regular Nerd Font Complete:size=14", 
										"Mononoki Nerd Font:size=14:antialias=true:autohint=true"
										};
static const char dmenufont[]       = "monospace:size=15";
static char normbgcolor[]           = "#2e3440";
static char normbordercolor[]       = "#2e3440";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#5e81ac";
static char selbgcolor[]            = "#5e81ac";
static char *colors[][3] = {
   	       /*               fg           bg           border   */
   	   [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
/* static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" }; */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */

/* layout(s) */
	/* class      		instance    title       tags mask     iscentered 	isfloating   isterminal		noswallow	monitor */
	{ "Gimp",    		NULL,       NULL,       0,            0,			1,           0, 			0,			-1 },
	{ "feh",     		NULL,       NULL,       0,            1,			0,           0, 			0,			-1 },
	{ "st",     		NULL,       NULL,       0,            1,			0,           1, 			-1,			-1 },
	{ "scratchpad",		NULL,       NULL,       0,            0,			1,           1, 			-1,			-1 },
	{ "Steam",     		NULL,       NULL,       4,            0,			0,           0, 			0,			-1 },
	{ "csgo_linux64", 	NULL,       NULL,       0,            0,			1,           0, 			0,			-1 },
	{ "float",     		NULL,       NULL,       0,            1,			1,           0, 			0,			-1 },
	{ "dragon",     		NULL,       NULL,       0,            1,			1,           0, 			0,			-1 },
	{ "Firefox",  		NULL,  "Firefox",       2,            0,			0,           0, 			-1,			-1 },
	{ NULL,      		NULL,   "Event Tester", 0,         	  0,			1, 	         0,           	1,	        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[ 侀 ]",     dwindle },
 	{ "[ 侀² ]",      spiral },
	{ "[  ]",      NULL },    /* no layout function means floating behavior */
	{ "[  ]",      monocle },
	{ "[ ﱢ ]",      tile },    /* first entry is default */
	{ "[ 冀 ]",      centeredmaster },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define FNKEY 0
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, /* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/* static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-p", "Run: ", NULL }; */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, "-p", "Run: ", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x28", NULL };
static const char *termfloat[]  = { "st","-c","float", NULL };
static const char *firefox[]  = { "firefox", NULL };
static const char *emacs[]  = { "emacs", NULL };
static const char *shutter[]  = { "shutter","-s", NULL };
static const char *neomutt[]  = { "st","-c","float","-e","neomuttwrap", NULL };
static const char *ranger[]  = { "st","-c","float","-e","ranger", NULL };
static const char *clipmenu[]  = { "clipmenu", NULL };
static const char *htop[]  = { "st","-c","float","-e","htop", NULL };
static const char *qr[]  = { "qr_wrap", NULL };
static const char *bar[]  = { "bar_update", NULL };
static const char *volUp[]  = { "amixer","-D","pulse","sset","Master","5%+", NULL };
static const char *volDown[]  = { "amixer","-D","pulse","sset","Master","5%-", NULL };
static const char *volMute[]  = { "amixer","set","Master","toggle", NULL };
static const char *micMute[]  = { "amixer","set","Capture","toggle", NULL };
static const char *brightUp[]  = { "light","-A","10", NULL };
static const char *brightDown[]  = { "light","-U","10", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      	spawn,          {.v = dmenucmd } },
	{ MODKEY,		        		XK_Return, 	spawn,          {.v = termcmd } },
	{ MODKEY,		        	XK_backslash, 	spawn,          {.v = termfloat } },
	{ MODKEY,                       XK_b,      	spawn,      	{.v = firefox } },
	{ MODKEY,                       XK_e,      	spawn,      	{.v = emacs } },
	{ MODKEY,                       XK_Print,  	spawn,      	{.v = shutter } },
	{ MODKEY,                       XK_m,      	spawn,      	{.v = neomutt } },
	{ MODKEY,                       XK_o,      	spawn,      	{.v = ranger } },
	{ MODKEY,                       XK_p,      	spawn,      	{.v = htop } },
	{ MODKEY,                       XK_c,      	spawn,      	{.v = clipmenu } },
	{ MODKEY,                       XK_F1,     	spawn,      	{.v = qr } },
	{ MODKEY,                       XK_F2,     	spawn,      	{.v = bar } },
	{ MODKEY,                       XK_s,	    togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_F5,      xrdb,          	{.v = NULL } },
	{ FNKEY,						XF86XK_AudioRaiseVolume,     spawn,			{.v = volUp } },
	{ FNKEY,						XF86XK_AudioLowerVolume,     spawn,			{.v = volDown } },
	{ FNKEY,						XF86XK_AudioMute,     		 spawn,			{.v = volMute } },
	{ FNKEY,						XF86XK_AudioMicMute,     	 spawn,			{.v = micMute } },
	{ FNKEY,						XF86XK_MonBrightnessUp,		 spawn,			{.v = brightUp } },
	{ FNKEY,						XF86XK_MonBrightnessDown,	 spawn,			{.v = brightDown } },
	{ MODKEY|ShiftMask,             XK_b,  	   	togglebar,      {0} }, 
	{ MODKEY,                       XK_minus,  	setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  	setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_equal,  	setgaps,        {.i = 0  } },
	{ MODKEY,                       XK_k,      	focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      	focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      	incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      	incnmaster,     {.i = -1 } },
	{ MODKEY,						XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY,           			XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_t,  		setlayout,      {.v = &layouts[0]} },
    /* { MODKEY,                       XK_f,    	setlayout,      {.v = &layouts[1]} }, */
	/* { MODKEY,                       XK_m,		setlayout,      {.v = &layouts[2]} }, */
	/* { MODKEY,                       XK_r,     	setlayout,      {.v = &layouts[3]} }, */
	/* { MODKEY|ShiftMask,             XK_r,     	setlayout,      {.v = &layouts[4]} }, */
	{ MODKEY|ShiftMask,             XK_space, 	togglefloating, {0} },
	{ MODKEY,                       XK_f,     	togglefullscr,  {0} },
	{ MODKEY,                       XK_0,     	view,           {.ui = ~0 } },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_backslash,   zoom,           {0} },
	{ MODKEY,                       XK_Tab,    	view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      	killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_0,      	tag,            {.ui = ~0 } },
	{ MODKEY,	                	XK_grave, 	focusmon,       {.i = +1 } },
	/* { MODKEY,    		        	XK_period,	focusmon,       {.i = -1 } }, */
	{ MODKEY|ShiftMask,             XK_grave,  	tagmon,         {.i = -1 } },
	/* { MODKEY|ShiftMask,             XK_period, 	tagmon,         {.i = +1 } }, */
	{ MODKEY|ShiftMask,             XK_j,	   	moveresize,     {.v = "0x 100y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_k,      	moveresize,     {.v = "0x -100y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_l,	   	moveresize,     {.v = "100x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_h,	   	moveresize,     {.v = "-100x 0y 0w 0h" } },
	{ MODKEY,		                XK_Down,   	moveresize,     {.v = "0x 0y 0w 100h" } },
	{ MODKEY,		                XK_Up,     	moveresize,     {.v = "0x 0y 0w -100h" } },
	{ MODKEY,		                XK_Right,  	moveresize,     {.v = "0x 0y 100w 0h" } },
	{ MODKEY,		                XK_Left,   	moveresize,     {.v = "0x 0y -100w 0h" } },
	TAGKEYS(                        XK_1,      	                0)
	TAGKEYS(                        XK_2,      	                1)
	TAGKEYS(                        XK_b,      	                1)
	TAGKEYS(                        XK_3,      	                2)
	TAGKEYS(                        XK_4,      	                3)
	TAGKEYS(                        XK_5,      	                4)
	TAGKEYS(                        XK_6,      	                5)
	TAGKEYS(                        XK_7,      	                6)
	TAGKEYS(                        XK_8,      	                7)
	TAGKEYS(                        XK_9,      	                8)
	{ MODKEY|ShiftMask,             XK_c,      	quit,           {0} },
	{ MODKEY|ShiftMask,             XK_x,      	quit,           {1} }, //restart
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
