#include "X11/XF86keysym.h"
/* See LICENSE file for copyright and license details. */

#include "f_push.c"
#include "l_fibonacci.c"
#include "l_gaplessgrid.c"

/* appearance */
static const char font[]            = "ProFont:size=9";

#define COLOR_URGENT	5
#define COLOR_FOCUS	2
#define NUMCOLORS         7             // need at least 3
static const char colors[NUMCOLORS][ColLast][8] = {
   // border   foreground  background
   { "#444444", "#dddddd", "#222222" },  // 0 = normal
   { "#5f5f5f", "#f8f8f8", "#5f5f5f" },  // 1 = selected
   { "#a592e2", "#a592e2", "#222222" },  // 2 = yuyublue
   { "#d4cf7b", "#d4cf7b", "#222222" },  // 3 = yuyuyellow
   { "#e2a5aa", "#e2a5aa", "#222222" },  // 4 = yuyupink
   { "#ff0000", "#ff0000", "#000000" },  // 5 - urgentred
   { "#aaaaaa", "#dddddd", "#222222" },  // 6 - focus
   // add more here
};

static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	//{ "Gimp",     NULL,       NULL,       0,            True,        -1 },
	{ "Firefox",  NULL,       NULL,       0 << 8,       False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[\\]",	dwindle },
	{ "[+]",	gaplessgrid },
};

//	{ "><>",      NULL },    /* no layout function means floating behavior */
//	{ "[M]",      monocle },

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
		{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", colors[0][ColBG], "-nf", colors[0][ColFG],"-sb", colors[1][ColBG], "-sf", colors[1][ColFG], NULL };
static const char *termcmd[]  = { "urxvt", NULL };
static const char *scrcmd[] = { "scrot", "/home/asie/Pictures/%Y-%m-%d-%T.png", NULL };
static const char *scrscmd[] = { "scrot", "-s", "/home/asie/Pictures/%Y-%m-%d-%T.png", NULL };
static const char *resscmd[] = { "xrandr-rescale" };
static const char *vrcmd[] = { "amixer", "set", "Master", "5%+" };
static const char *vlcmd[] = { "amixer", "set", "Master", "5%-" };
static Key keys[] = {
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
/*	{ 0,				XF86XK_AudioRaiseVolume,	spawn,	{.v = vrcmd}},
	{ 0,				XF86XK_AudioLowerVolume,	spawn,	{.v = vlcmd}}, */
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_m,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_z,      togglefloating,      {0} },
	{ MODKEY|ShiftMask,                       XK_m,      tagcycle,     {.i = +1 } },
	{ MODKEY|ShiftMask,                       XK_n,      tagcycle,     {.i = -1 } },
	{ MODKEY,                       XK_p,      cycle,     {.i = +1 } },
	{ MODKEY,                       XK_q,      cycle,     {.i = -1 } },
	{ MODKEY,                       XK_i,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_e,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_w,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_a,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_k,      pushup,		{0} },
	{ MODKEY,                       XK_s,      pushdown,		{0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,			XK_space,	   nextlayout,	   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
//	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_f,	spawn,		{.v = resscmd} },
	{ 0,			0xff61,	spawn,		{.v = scrcmd} },
	{ ShiftMask,			0xff61,	spawn,		{.v = scrscmd} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,                       XK_q,      quit,           {1} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        nextlayout,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

