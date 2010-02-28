/*
** Login : <ctaf42@gmail.com>
** Started on  Sun Jan 27 18:35:38 2008 Cedric GESTES
** $Id$
**
** Author(s)
**  - Cedric GESTES <ctaf42@gmail.com>
**  - Fabrice REY
**
** Copyright (C) 2008 Cedric GESTES
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef __CAIRO_DESKLET_H__
#define  __CAIRO_DESKLET_H__

#include "cairo-dock-struct.h"
#include "cairo-dock-surface-factory.h"
#include "cairo-dock-load.h"
#include "cairo-dock-container.h"
G_BEGIN_DECLS


/**
*@file cairo-dock-desklet.h This class defines the Desklets, that are Widgets placed directly on your desktop.
* A Desklet is a container that holds 1 applet's icon plus an optionnal list of other icons and an optionnal GTK widget, has a decoration, suports several accessibility types (like Compiz Widget Layer), and has a renderer.
* Desklets can be resized or moved directly with the mouse, and can be rotated in the 3 directions of space.
*/

/// Type of accessibility of a Desklet.
typedef enum {
	/// Normal, like normal window
	CAIRO_DESKLET_NORMAL = 0,
	/// always above
	CAIRO_DESKLET_KEEP_ABOVE,
	/// always below
	CAIRO_DESKLET_KEEP_BELOW,
	/// on the Compiz widget layer
	CAIRO_DESKLET_ON_WIDGET_LAYER,
	/// prevent other windows form overlapping it
	CAIRO_DESKLET_RESERVE_SPACE
	} CairoDeskletAccessibility;

/// Decoration of a Desklet.
struct _CairoDeskletDecoration {
	const gchar *cDisplayedName;
	gchar *cBackGroundImagePath;
	gchar *cForeGroundImagePath;
	CairoDockLoadImageModifier iLoadingModifier;
	gdouble fBackGroundAlpha;
	gdouble fForeGroundAlpha;
	gint iLeftMargin;
	gint iTopMargin;
	gint iRightMargin;
	gint iBottomMargin;
	};

/// Configuration attributes of a Desklet.
struct _CairoDeskletAttribute {
	gboolean bDeskletUseSize;
	gint iDeskletWidth;
	gint iDeskletHeight;
	gint iDeskletPositionX;
	gint iDeskletPositionY;
	gboolean bPositionLocked;
	gint iRotation;
	gint iDepthRotationY;
	gint iDepthRotationX;
	gchar *cDecorationTheme;
	CairoDeskletDecoration *pUserDecoration;
	CairoDeskletAccessibility iAccessibility;
	gboolean bOnAllDesktops;
	gint iNumDesktop;
	gboolean bNoInput;
} ;

typedef gpointer CairoDeskletRendererDataParameter;
typedef CairoDeskletRendererDataParameter* CairoDeskletRendererDataPtr;
typedef gpointer CairoDeskletRendererConfigParameter;
typedef CairoDeskletRendererConfigParameter* CairoDeskletRendererConfigPtr;
typedef struct {
	gchar *cName;
	CairoDeskletRendererConfigPtr pConfig;
} CairoDeskletRendererPreDefinedConfig;
typedef void (* CairoDeskletRenderFunc) (cairo_t *pCairoContext, CairoDesklet *pDesklet);
typedef void (*CairoDeskletGLRenderFunc) (CairoDesklet *pDesklet);
typedef gpointer (* CairoDeskletConfigureRendererFunc) (CairoDesklet *pDesklet, cairo_t *pSourceContext, CairoDeskletRendererConfigPtr pConfig);
typedef void (* CairoDeskletLoadRendererDataFunc) (CairoDesklet *pDesklet, cairo_t *pSourceContext);
typedef void (* CairoDeskletUpdateRendererDataFunc) (CairoDesklet *pDesklet, CairoDeskletRendererDataPtr pNewData);
typedef void (* CairoDeskletFreeRendererDataFunc) (CairoDesklet *pDesklet);
typedef void (* CairoDeskletLoadIconsFunc) (CairoDesklet *pDesklet, cairo_t *pSourceContext);
/// Definition of a Desklet's renderer.
struct _CairoDeskletRenderer {
	/// rendering function with libcairo.
	CairoDeskletRenderFunc 				render;
	/// rendering function with OpenGL.
	CairoDeskletGLRenderFunc 			render_opengl;
	/// get the configuration of the renderer from a set of config attributes.
	CairoDeskletConfigureRendererFunc 	configure;
	/// load the internal data of the renderer.
	CairoDeskletLoadRendererDataFunc 	load_data;
	/// free all internal data of the renderer.
	CairoDeskletFreeRendererDataFunc 	free_data;
	/// define the icons' size and load them.
	CairoDeskletLoadIconsFunc 			load_icons;
	/// function called on each iteration of the rendering loop.
	CairoDeskletUpdateRendererDataFunc 	update;
	/// optionnal rendering function with OpenGL that only draws the bounding boxes of the icons (for picking).
	CairoDeskletGLRenderFunc 			render_bounding_box;
	/// An optionnal list of preset configs.
	GList *pPreDefinedConfigList;
};


/// Definition of a Desklet, which derives from a Container.
struct _CairoDesklet {
	//\________________ Core
	// container
	CairoContainer container;
	// L'icone de l'applet.
	Icon *pIcon;
	// Liste eventuelle d'icones placees sur le desklet, et susceptibles de recevoir des clics.
	GList *icons;
	// le moteur de rendu utilise pour dessiner le desklet.
	CairoDeskletRenderer *pRenderer;
	// donnees pouvant etre utilisees par le moteur de rendu.
	gpointer pRendererData;
	// The following function outclasses the corresponding function of the renderer. This is useful if you don't want to pick icons but some elements that you draw yourself on the desklet.
	CairoDeskletGLRenderFunc render_bounding_box;
	// ID of the object that was picked in case the previous function is not null.
	GLuint iPickedObject;
	
	//\________________ decorations
	gchar *cDecorationTheme;
	CairoDeskletDecoration *pUserDecoration;
	gint iLeftSurfaceOffset;
	gint iTopSurfaceOffset;
	gint iRightSurfaceOffset;
	gint iBottomSurfaceOffset;
	CairoDockImageBuffer backGroundImageBuffer;
	CairoDockImageBuffer foreGroundImageBuffer;
	
	//\________________ properties.
	gdouble fRotation;  // rotation.
	gdouble fDepthRotationY;
	gdouble fDepthRotationX;
	gboolean bFixedAttitude;
	gboolean bAllowNoClickable;
	gboolean bNoInput;
	GtkWidget *pInteractiveWidget;
	gboolean bPositionLocked;  // TRUE ssi on ne peut pas deplacer le widget a l'aide du simple clic gauche.
	
	//\________________ internal
	gint iSidWritePosition;  // un timer pour retarder l'ecriture dans le fichier lors des deplacements.
	gint iSidWriteSize;  // un timer pour retarder l'ecriture dans le fichier lors des redimensionnements.
	gint iDesiredWidth, iDesiredHeight;  // taille a atteindre (fixee par l'utilisateur dans le.conf)
	gint iKnownWidth, iKnownHeight;  // taille connue par l'applet associee.
	gboolean bSpaceReserved;  // l'espace est actuellement reserve.
	gboolean bAllowMinimize;  // TRUE to allow the desklet to be minimized once. The flag is reseted to FALSE after the desklet has minimized.
	gint iMouseX2d;  // X position of the pointer taking into account the 2D transformations on the desklet (for an opengl renderer, you'll have to use the picking).
	gint iMouseY2d;  // Y position of the pointer taking into account the 2D transformations on the desklet (for an opengl renderer, you'll have to use the picking).
	GTimer *pUnmapTimer;
	gdouble fButtonsAlpha;  // pour le fondu des boutons lors de l'entree dans le desklet.
	gboolean bButtonsApparition;  // si les boutons sont en train d'apparaitre ou de disparaitre.
	gboolean bGrowingUp;  // pour le zoom initial.
	
	//\________________ current state
	gboolean rotatingY;
	gboolean rotatingX;
	gboolean rotating;
	gboolean retaching;  // rattachement au dock.
	gboolean making_transparent;  // no input.
	gboolean moving;  // pour le deplacement manuel de la fenetre.
	gboolean bClicked;
	guint time;  // date du clic.
};

/// Definition of a function that runs through all desklets.
typedef gboolean (* CairoDockForeachDeskletFunc) (CairoDesklet *pDesklet, CairoDockModuleInstance *pInstance, gpointer data);


/** Say if a Container is a desklet.
*@param pContainer the container.
*@return TRUE if the container is a desklet.
*/
#define CAIRO_DOCK_IS_DESKLET(pContainer) (pContainer != NULL && CAIRO_CONTAINER(pContainer)->iType == CAIRO_DOCK_TYPE_DESKLET)
/**
* Cast a Container into a Desklet.
*@param pContainer the container.
*@return the desklet.
*/
#define CAIRO_DESKLET(pContainer) ((CairoDesklet *)pContainer)

void cairo_dock_init_desklet_manager (void);
void cairo_dock_load_desklet_buttons (void);
void cairo_dock_unload_desklet_buttons (void);

void cairo_dock_reload_desklets_decorations (gboolean bDefaultThemeOnly, cairo_t *pSourceContext);
void cairo_dock_free_desklet_decoration (CairoDeskletDecoration *pDecoration);


/** Make all desklets visible. Their accessibility is set to #CAIRO_DESKLET_NORMAL. 
*@param bOnWidgetLayerToo TRUE if you want to act on the desklet that are on the WidgetLayer as well.
*/
void cairo_dock_set_all_desklets_visible (gboolean bOnWidgetLayerToo);
/** Reset the desklets accessibility to the state defined in their conf file.
*/
void cairo_dock_set_desklets_visibility_to_default (void);

/** Get the desklet whose X ID matches the given one.
*@param Xid an X ID.
*@return the desklet that matches, or NULL if none match.
*/
CairoDesklet *cairo_dock_get_desklet_by_Xid (Window Xid);


/** Create a simple desklet, without placing it nor defining a renderer.
*@param pIcon the main icon.
*@param pInteractiveWidget an optionnal GTK widget, or NULL if none.
*@param iAccessibility accessibility of the desklet, needed to know if it should be placed on the WidgetLayer or if it should reserve its space.
*@return the newly allocated desklet.
*/
CairoDesklet *cairo_dock_create_desklet (Icon *pIcon, GtkWidget *pInteractiveWidget, CairoDeskletAccessibility iAccessibility);

/** Destroy a desklet, and free all the allocated ressources. The interactive widget is removed before, and can be inserted anywhere after that.
*@param pDesklet the desklet to destroy.
*/
void cairo_dock_free_desklet (CairoDesklet *pDesklet);

/** Configure a un desklet.
* It places it, resizes it, sets up its accessibility, locks its position, and sets up its decorations.
*@param pDesklet the desklet.
*@param pAttribute the attributes to configure the desklet.
*/
void cairo_dock_configure_desklet (CairoDesklet *pDesklet, CairoDeskletAttribute *pAttribute);

#define cairo_dock_set_static_desklet(pDesklet) (pDesklet)->bFixedAttitude = TRUE

#define cairo_dock_allow_no_clickable_desklet(pDesklet) (pDesklet)->bAllowNoClickable = TRUE


void cairo_dock_reserve_space_for_desklet (CairoDesklet *pDesklet, gboolean bReserve);


/** Add a GtkWidget to a desklet. Only 1 widget is allowed per desklet, if you need more, you can just use a GtkContainer, and place as many widget as you want inside.
*@param pInteractiveWidget the widget to add.
*@param pDesklet the desklet.
*@param iRightMargin right margin, in pixels, useful to keep a clickable zone on the desklet, or 0 if you don't want a margin.
*/
void cairo_dock_add_interactive_widget_to_desklet_full (GtkWidget *pInteractiveWidget, CairoDesklet *pDesklet, int iRightMargin);
/** Add a GtkWidget to a desklet. Only 1 widget is allowed per desklet, if you need more, you can just use a GtkContainer, and place as many widget as you want inside.
*@param pInteractiveWidget the widget to add.
*@param pDesklet the desklet.
*/
#define cairo_dock_add_interactive_widget_to_desklet(pInteractiveWidget, pDesklet) cairo_dock_add_interactive_widget_to_desklet_full (pInteractiveWidget, pDesklet, 0)
/** Sezt the right margin of a desklet. This is useful to keep a clickable zone on the desklet when you put a GTK widget inside.
*@param pDesklet the desklet.
*@param iRightMargin right margin, in pixels.
*/
void cairo_dock_set_desklet_margin (CairoDesklet *pDesklet, int iRightMargin);

/** Detach the interactive widget from a desklet. The widget can then be placed anywhere after that. You have to unref it after you placed it into a container, or to destroy it.
*@param pDesklet the desklet with an interactive widget.
*@return the widget.
*/
GtkWidget *cairo_dock_steal_interactive_widget_from_desklet (CairoDesklet *pDesklet);


/** Find the currently pointed icon in a desklet, taking into account the 3D rotations.
*@param pDesklet the desklet.
*@return the pointed icon, or NULL if none.
*/
Icon *cairo_dock_find_clicked_icon_in_desklet (CairoDesklet *pDesklet);


/** Hide a desklet.
*@param pDesklet the desklet.
*/
void cairo_dock_hide_desklet (CairoDesklet *pDesklet);
/**
* Show a desklet, and give it the focus.
*@param pDesklet the desklet.
*/
void cairo_dock_show_desklet (CairoDesklet *pDesklet);


/** Launch a "zoom out" animation on a desklet.
*@param pDesklet the desklet.
*/
void cairo_dock_zoom_out_desklet (CairoDesklet *pDesklet);



G_END_DECLS

#endif
