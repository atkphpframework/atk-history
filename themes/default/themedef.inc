<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage themes
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /** Theme definition */

  $theme["customtpls"] = "scan 'templates/'";  
  $theme["customstyles"] = "scan 'styles/'";
  $theme['customimgs'] = "scan 'images/'";
  
  $theme['recordlist_icons'] = "scan 'icons/recordlist/'";
  $theme['toolbar_icons'] = "scan 'icons/toolbar/'";
  
  $theme['highlight'] = '#eeeeee';
  $theme['select'] = '#cccccc';
  
  // Outlook style menu
  $theme['ol_menucolor']='#ffffff';
  $theme['ol_wincolor']='#eeeee0';
  $theme['ol_background']='#677EB1';
  
  $theme['doctype'] = '<?xml version="1.0" encoding="UTF-8"?>'."\n".
                      '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"'.
                      ' "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">';
                      
  $theme['menu']= array(
    'plain' => array(
      'delimiter' => "<br>",
      'pos' => "left",
      'menu_align' => "center",
      ),
    'theme default'=>'plain'
  );
  
  $theme['top_frame'] = 0;
  $theme['recordlist_orientation']  = "left";
  $theme['recordlist_vorientation'] = "middle";
  $theme['recordlist_icons'] = "true";
  
?>
