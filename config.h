#include <X11/XF86keysym.h>
#include "fibonacci.c"
#include "movestack.c"
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int barpadv   = 18;
static const unsigned int barpadh   = 18;
static const unsigned int barheight = 0;       /* height of the bar */
static const unsigned int barborder = 1;	/* border pixel of the bar */
static const unsigned int floatbar  = 1;        /* 0 means no bar on floating windows */
static const unsigned int gappx     = 18;
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack Nerd Font:size=10" };
static const char dmenufont[]       = "Hack Nerd Font:size=10";
static const char col_dark[]        = "#1a1b26";
static const char col_mid[]         = "#565f89";
static const char col_bright[]      = "#9aa5ce";
static const char col_highlight[]   = "#73daca";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_bright, col_dark, col_mid },
	[SchemeSel]  = { col_dark, col_highlight,  col_highlight  },
	[SchemeBar]  = { NULL, NULL, col_highlight }, /* bar colors */
};

/* tagging */
static const char *tags[] = { "1: main", "2: code", "3: music", "4: gfx", "5: vms", "6: games", "7: msg" };
static const int taglayouts[] =  { 0, 3, 0, 0, 0, 0, 0 };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                    instance    title       tags mask     isfloating   monitor		border width */
	{ "librewolf",              NULL,       NULL,       1 << 0,       0,           -1,		-1 },
	{ "com.bitwig.BitwigStudio",NULL,       NULL,       1 << 2,       0,           -1,		-1 },
	{ "Gimp",                   NULL,       NULL,       1 << 3,       0,           -1,		-1 },
	{ "Inkscape",               NULL,       NULL,       1 << 3,       0,           -1,		-1 },
	{ "qemu-system-x86_64",     NULL,       NULL,       1 << 4,       0,           -1,		-1 },
	{ "steam",                  NULL,       NULL,       1 << 5,       0,           -1,		-1 },
	{ "cstrike_linux64",        NULL,       NULL,       1 << 5,       0,           -1,		 0 },
	{ "discord",                NULL,       NULL,       1 << 6,       0,           -1,		-1 },
	{ "TelegramDesktop",        NULL,       NULL,       1 << 6,       0,           -1,		-1 },
	{ "thunderbird-default",    NULL,       NULL,       1 << 6,       0,           -1,		-1 },

	{ "Ternar Music Technology-", NULL, NULL, 0, 1,-1}
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[\\]",     dwindle },
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_dark, "-nf", col_bright, "-sb", col_highlight, "-sf", col_dark, NULL };
static const char *termcmd[]  = { "st", NULL };
// Volume control commands
static const char *volup[]   = { "/bin/sh", "-c", "pactl set-sink-volume $(pactl get-default-sink) +5% && /home/chris/dev/suckless/dwm/dwm-update.sh", NULL };
static const char *voldown[] = { "/bin/sh", "-c", "pactl set-sink-volume $(pactl get-default-sink) -5% && /home/chris/dev/suckless/dwm/dwm-update.sh", NULL };
static const char *volmute[] = { "/bin/sh", "-c", "pactl set-sink-mute $(pactl get-default-sink) toggle && /home/chris/dev/suckless/dwm/dwm-update.sh", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_plus,  setgaps,        {.i = +1 } },
	{ MODKEY,                       XK_plus,  setgaps,        {.i = 0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ 0, XF86XK_AudioRaiseVolume, 	spawn, {.v = volup   } },
	{ 0, XF86XK_AudioLowerVolume, 	spawn, {.v = voldown } },
	{ 0, XF86XK_AudioMute,        	spawn, {.v = volmute } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

