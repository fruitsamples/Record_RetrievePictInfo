/*	File:		Record RetrievePictInfo.c	Contains:	This snippet shows how to retrieve the colortable					from multiple PICT resources using the System 7 calls				RecordPictInfo and RetrievePictInfo.	Written by: Edgar Lee		Copyright:	Copyright © 1992-1999 by Apple Computer, Inc., All Rights Reserved.				You may incorporate this Apple sample source code into your program(s) without				restriction. This Apple sample source code has been provided "AS IS" and the				responsibility for its operation is yours. You are not permitted to redistribute				this Apple sample source code as "Apple sample source code" after having made				changes. If you're going to re-distribute the source, we require that you make				it clear in the source that the code was descended from Apple sample source				code, but that you've made changes.	Change History (most recent first):				7/13/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1				*/#include <Dialogs.h>#include <Fonts.h>#include <PictUtils.h>#include <TextUtils.h>/* Constant Declarations */#define	WWIDTH	512#define	WHEIGHT	200#define	MAXPICTS	6		/* Maximum PICT resources used for the example. */#define WLEFT	(((qd.screenBits.bounds.right - qd.screenBits.bounds.left) - WWIDTH) / 2)#define WTOP	(((qd.screenBits.bounds.bottom - qd.screenBits.bounds.top) - WHEIGHT) / 2)void initMac();void createWindow();void retrieveColors();void doEventLoop();void main(void){	initMac();		createWindow();		doEventLoop();}void initMac(){	MaxApplZone();	InitGraf( &qd.thePort );	InitFonts();	InitWindows();	InitMenus();	TEInit();	InitDialogs( nil );	InitCursor();	FlushEvents( 0, everyEvent );}void createWindow(){	Rect		rect;	WindowPtr	window;		SetRect( &rect, WLEFT, WTOP, WLEFT + WWIDTH, WTOP + WHEIGHT );	window = NewCWindow( 0L, &rect, "\pRecord•RetrievePictInfo", true, documentProc,							(WindowPtr)-1L, true, 0L );							SetPort( window );		TextMode( srcCopy );	TextFont( kFontIDGeneva);	TextSize( 9 );		PenSize( 2, 1 );}void retrieveColors(){	int				i;	RGBColor		color;	PicHandle		pict[MAXPICTS];	PictInfoID		pictInfoID;	PictInfo		pictInfo;	CTabHandle		ctable;	PaletteHandle	palette;	Rect			rect;	Str255			string;		/* Request a unique ID for our PictInfo. */	NewPictInfo( &pictInfoID, returnColorTable, 256, popularMethod, 0 );		MoveTo( 10, 30 );	DrawString( "\pRecording PictInfo" );	MoveTo( 10, 45 );	DrawString( "\pfrom PICT resources:" );		/* Loop through the PICT resources. */	for (i = 0; i < MAXPICTS; i++)	{		/* Load the PICT. */		pict[i] = GetPicture( 128 + i );		HPurge( (Handle)pict[i] );				/* Define the PICT's destination rect then draw into it. */		rect = (**pict[i]).picFrame;		OffsetRect( &rect, -rect.left, -rect.top );		rect.right /= 2;		rect.bottom /= 2;				OffsetRect( &rect, rect.right * (i + 2), 0 );		DrawPicture( pict[i], &rect );				/* Draw the PICT's ID below its image. */		MoveTo( rect.left + 20, rect.bottom + 10 );		NumToString( (long)(128 + i), string );		DrawString( string );				/* Accumulate information about this PICT. */		RecordPictInfo( pictInfoID, pict[i] );	}	MoveTo( 10, 95 );	DrawString( "\pColortable returned from RetrievePictInfo:" );	/* Retrieve the colortable from the recording. */	RetrievePictInfo( pictInfoID, &pictInfo, 256 );	ctable = pictInfo.theColorTable;		/* Convert the colortable to a palette then attach it to the window. */	palette = NewPalette( 0, nil, pmTolerant + pmExplicit, 0 );	CTab2Palette( ctable, palette, pmTolerant + pmExplicit, 0 );	SetPalette( FrontWindow(), palette, false );		/* Draw the colortable in the window to see what it looks like. */	for (i = 0; i < 256; i++)	{		color = (**ctable).ctTable[i].rgb;				RGBForeColor( &color );		MoveTo( i * 2, 100 );		LineTo( i * 2, WHEIGHT );	}		/* Release any used memory. */	DisposePictInfo( pictInfoID );	DisposeCTable( ctable );	DisposePalette( palette );}void doEventLoop(){	EventRecord event;	WindowPtr   window;	short       clickArea;	Rect        screenRect;	for (;;)	{		if (WaitNextEvent( everyEvent, &event, 0, nil ))		{			if (event.what == mouseDown)			{				clickArea = FindWindow( event.where, &window );								if (clickArea == inDrag)				{					screenRect = (**GetGrayRgn()).rgnBBox;					DragWindow( window, event.where, &screenRect );				}				else if (clickArea == inContent)				{					if (window != FrontWindow())						SelectWindow( window );				}				else if (clickArea == inGoAway)					if (TrackGoAway( window, event.where ))						return;			}			else if (event.what == updateEvt)			{				window = (WindowPtr)event.message;					SetPort( window );								BeginUpdate( window );				retrieveColors();				EndUpdate( window );			}		}	}}