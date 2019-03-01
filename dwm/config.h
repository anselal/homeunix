#include "nerdfonts.h"
#include "selfrestart.c"

/* See LICENSE file for copyright and license details. */
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 5;        /* pixel gap between clients */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 6;        /* horizontal padding for statusbar */
static const int vertpadbar         = 7;        /* vertical padding for statusbar */
static const char *fonts[]          = { "UbuntuMono Nerd Font:size=10", "Font Awesome 5 Brands:size=10" };
static const char dmenufont[]       = "UbunutMono Nerd Font:size=10";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#000000"; /* border color unfocused windows */
static const char col_gray3[]       = "#96b5b4";
static const char col_gray4[]       = "#c0c5ce";
static const char col_cyan[]        = "#924441"; /* border color focused windows and tags */
static const unsigned int baralpha = 0xbe;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray4, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { N_TERMINAL, N_FIREFOX, N_CODE, N_FOLDER, N_DESKTOP, N_GIT, N_C, N_PYTHON, N_SKYPE  };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask     isfloating   monitor */
	{ "Firefox",        NULL,       NULL,       1 << 1,       0,           -1 },
	{ "Gentoo",         NULL,       NULL,       1 << 3,       0,           -1 },
	{ "Remmina",        NULL,       NULL,       1 << 4,       0,           -1 },
	{ "st-256color",    NULL,       "htop",     0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define CMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier               key              function        argument */
	{ MODKEY,                 XK_z,            spawn,          {.v = dmenucmd } },
	{ MODKEY,                 XK_Return,       spawn,          {.v = termcmd } },
	{ MODKEY,                 XK_b,            togglebar,      {0} },
	{ MODKEY|ShiftMask,       XK_j,            rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,       XK_k,            rotatestack,    {.i = -1 } },
	{ MODKEY,                 XK_j,            focusstack,     {.i = +1 } },
	{ MODKEY,                 XK_k,            focusstack,     {.i = -1 } },
	{ MODKEY,                 XK_i,            incnmaster,     {.i = +1 } },
	{ MODKEY,                 XK_d,            incnmaster,     {.i = -1 } },
	{ MODKEY,                 XK_h,            setmfact,       {.f = -0.05} },
	{ MODKEY,                 XK_l,            setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,       XK_Return,       zoom,           {0} },
	{ MODKEY,                 XK_Tab,          view,           {0} },
	{ MODKEY|ShiftMask,       XK_c,            killclient,     {0} },
	{ MODKEY,                 XK_t,            setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                 XK_f,            setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                 XK_m,            setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                 XK_g,            setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ControlMask,     XK_comma,        cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,     XK_period,       cyclelayout,    {.i = +1 } },
	{ MODKEY,                 XK_space,        setlayout,      {0} },
	{ MODKEY|ShiftMask,       XK_space,        togglefloating, {0} },
	{ MODKEY,                 XK_0,            view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,       XK_0,            tag,            {.ui = ~0 } },
	{ MODKEY,                 XK_comma,        focusmon,       {.i = -1 } },
	{ MODKEY,                 XK_period,       focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,       XK_comma,        tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,       XK_period,       tagmon,         {.i = +1 } },
	{ ControlMask|ShiftMask,  XK_Escape,       spawn,          CMD("st -e htop") },
	
    /* Apps Launched with <SUPER> + <KEYPAD 1-9>  */
	{ MODKEY,                 XK_KP_End,       spawn,          CMD("") },
	{ MODKEY,                 XK_KP_Down,      spawn,          CMD("") },
	{ MODKEY,                 XK_KP_Page_Down, spawn,          CMD("") },
	{ MODKEY,                 XK_KP_Left,      spawn,          CMD("") },
	{ MODKEY,                 XK_KP_Begin,     spawn,          CMD("") },
	{ MODKEY,                 XK_KP_Right,     spawn,          CMD("") },
	{ MODKEY,                 XK_KP_Home,      spawn,          CMD("") },
	{ MODKEY,                 XK_KP_Up,        spawn,          CMD("") },
	{ MODKEY,                 XK_KP_Page_Up,   spawn,          CMD("") },

	
    /* Apps Launched with <SUPER> + <SHIFT> + <KEYPAD 1-9>  */
	{ MODKEY|ShiftMask,       XK_KP_End,       spawn,          CMD("") },
	{ MODKEY|ShiftMask,       XK_KP_Down,      spawn,          CMD("") },
	{ MODKEY|ShiftMask,       XK_KP_Page_Down, spawn,          CMD("") },
	{ MODKEY|ShiftMask,       XK_KP_Left,      spawn,          CMD("") },
	{ MODKEY|ShiftMask,       XK_KP_Begin,     spawn,          CMD("") },
	{ MODKEY|ShiftMask,       XK_KP_Right,     spawn,          CMD("") },
	{ MODKEY|ShiftMask,       XK_KP_Home,      spawn,          CMD("") },
	{ MODKEY|ShiftMask,       XK_KP_Up,        spawn,          CMD("") },
	{ MODKEY|ShiftMask,       XK_KP_Page_Up,   spawn,          CMD("") },
	
    /* Apps Launched with <SUPER> + <CONTROL> + <KEYPAD 1-9>  */
	{ MODKEY|ControlMask,     XK_KP_End,       spawn,          CMD("st -e htop") },
	{ MODKEY|ControlMask,     XK_KP_Down,      spawn,          CMD("") },
	{ MODKEY|ControlMask,     XK_KP_Page_Down, spawn,          CMD("") },
	{ MODKEY|ControlMask,     XK_KP_Left,      spawn,          CMD("") },
	{ MODKEY|ControlMask,     XK_KP_Begin,     spawn,          CMD("") },
	{ MODKEY|ControlMask,     XK_KP_Right,     spawn,          CMD("") },
	{ MODKEY|ControlMask,     XK_KP_Home,      spawn,          CMD("") },
	{ MODKEY|ControlMask,     XK_KP_Up,        spawn,          CMD("") },
	{ MODKEY|ControlMask,     XK_KP_Page_Up,   spawn,          CMD("") },
	
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,		quit,			{0} },
	{ MODKEY|ShiftMask, 			XK_r, 		self_restart,	{0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

