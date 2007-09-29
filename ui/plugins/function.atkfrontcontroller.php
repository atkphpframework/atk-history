<?php
/**
 * This file is part of the Achievo ATK distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package atk
 * @subpackage ui
 *
 * @author Peter C. Verhage <peter@ibuildings.nl>   
 *
 * @copyright (c) 2007 Ibuildings.nl
 * @license http://www.achievo.org/atk/licensing ATK Open Source License
 *
 * @version $Revision$
 * $Id$
 */
 
/**
 * atkFrontController plug-in.
 * 
 * The atkFrontController plug-in can be used either as dispatcher for the
 * root front controller (if for example nested on a WDE page) or to
 * instantiate a nested controller which output will be embedded in the
 * result of a parent controller's action.
 *
 * If no current front controller exists the plug-in will instantiate the 
 * root controller. For the root controller we also need look at the request 
 * (GET, POST) parameters. They will precede any given plug-in parameter. We 
 * also let the router process the request first so that it can rewrite the 
 * path parameter to the correct controller, action etc. Rewrites will be
 * explicitly turned off so that any follow-up request will be processed by
 * the plug-in.
 * 
 * If a root controller already exists, the plug-in is embedded inside another
 * controller's action template. In that case all request variables will be
 * ignored and no parsing of the path parameter will be done. However the
 * nested controller will have access to any of the request variables of it's
 * parent controller that are not overwritten by any of the plug-in parameters.
 *
 * @param array  $params "request" variables
 * @param Smarty $smarty smarty instance
 * 
 * @return string result result of the controller action
 */
function smarty_function_atkfrontcontroller($params, $smarty)
{
  atkimport('atk.front.atkfrontcontroller');
  return atkFrontController::dispatchRequest($params);
}