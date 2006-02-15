<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage menu
   * @author Peter Verhage <peter@ibuildings.nl>
   *
   * @todo Move this to the menu class that wraps this code.
   *
   * Builds a plain text navigation menu for the ATK application. 
   * Items for the main application can be added within the
   * config.menu.inc file with the menuitem() method. Modules
   * can register menuitems in their constructor. The menu
   * has support for enabling/disabling items on a user profile base.
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   */  
  
  /** @internal defines etc. */
  global $g_menu, $atkmenutop, $g_menu_parent, $g_sessionManager, $selectedprogram;
  
  atkimport("atk.ui.atkpage");
  $page = &atkPage::getInstance();
  $ui =   &atknew("atk.ui.atkui");
  $theme = &atkTheme::getInstance();
  $selectedprogram_session = $g_sessionManager->stackVar("selectedprogram");
  if (!$selectedprogram_session && $selectedprogram) $g_sessionManager->stackVar("selectedprogram",$selectedprogram);
  if (!$selectedprogram && $selectedprogram_session) $selectedprogram = $selectedprogram_session;

  /* output html */
//  $menu = "<div align='".atkconfig("menu_align", "center")."'>"; 
  
  /* build menu */
  
  /**
   * @ignore
   */
  function menu_cmp($a,$b)
  {
    if ($a["order"] == $b["order"]) return 0;
    return ($a["order"] < $b["order"]) ? -1 : 1;
  }
  
  if (is_array($g_menu[$atkmenutop]))
  {
    usort($g_menu[$atkmenutop],"menu_cmp");   
  
    for ($i = 0; $i < count($g_menu[$atkmenutop]); $i++)
    {
    	$name = $g_menu[$atkmenutop][$i]["name"];
      $url = $g_menu[$atkmenutop][$i]["url"];
      $enable = $g_menu[$atkmenutop][$i]["enable"];
      
      if ($i==count($g_menu[$atkmenutop])-1)
      {
        $delimiter = "";
      }
      else
      {
      	$delimiter = sprintf(atkconfig("menu_delimiter"),session_url($url, SESSION_NEW), 'main');
      }
      
      if (is_array($enable))
      {
        $enabled = false;
        for ($j=0;$j<(count($enable)/2);$j++)
        {
          $enabled |= is_allowed($enable[(2*$j)],$enable[(2*$j)+1]);
        }
        $enable = $enabled;
      }
  
      /* delimiter ? */
      if ($g_menu[$atkmenutop][$i]["name"] == "-") $menu .= "</td></tr><tr><td><hr />".$delimiter;
      
      /* submenu ? */
      else if (empty($url) && $enable) $menu .= sprintf(atkconfig("menu_delimiter"), session_url("menu.php?atkmenutop=$name&selectedprogram=$selectedprogram", SESSION_NEW), 
'menu')."<b>".text("menu_$name")."</b>".$delimiter;
      else if (empty($url) && !$enable)
      {
        $menu .=$delimiter.text("menu_$name");
      }
        
      /* normal menu item */
      else if ($enable) $menu .= $delimiter."<b>".text("menu_$name")."</b>";
      else 
      {
        $menu .= $delimiter.text("menu_$name");
      }
    }
  }
  /* previous */  
  if ($atkmenutop != "main")
  {
    $parent = $g_menu_parent[$atkmenutop];
    $menu .= sprintf(atkconfig("menu_delimiter"), session_url("menu.php?atkmenutop=$parent&selectedprogram=$selectedprogram",SESSION_NEW),'menu');
    $menu .= "<b>".text("back_to").' '.text("menu_$parent")."</b>";
  }      
  
  $page->register_style($theme->stylePath("style.css"));
  $page->register_style($theme->stylePath("menu.css"));
  //$box = $ui->renderBox(array("title"=>text("menu_".$atkmenutop),"content"=>$menu));
  $box = $ui->render("menu.tpl",array("title"=>text("menu_".$atkmenutop),"content"=>$menu));
 
  $page->addContent($box);
      
  $string = $page->render("Menu", true);
  $output = &atkOutput::getInstance();
  $output->output($string);
  $output->outputFlush();

?>
