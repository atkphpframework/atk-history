<?PHP
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage menu
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */
  atkimport('atk.menu.atkplainmenu');

  /**
   * Implementation of the Dropdowntext menu.
   *
   * @author Ber Dohmen <ber@ibuildings.nl>
   * @author Sandy Pleyte <sandy@ibuildings.nl>
   * @package atk
   * @subpackage menu
   */
  class atkDropdownMenu extends atkPlainMenu 
  {
    function render()
    {      
      $page = &atkinstance("atk.ui.atkpage");
      $menu = $this->load();
      $page->addContent($menu);

      return $page->render("Menu", true,' leftmargin="0" topmargin="0" marginheight="0" marginwidth="0"');
    }
    
    function load()
    {
      global $ATK_VARS,$g_menu;
      
      $page = &atkinstance('atk.ui.atkpage');
      $theme = &atkinstance("atk.ui.atktheme");
      $page->register_script(atkconfig('atkroot').'atk/javascript/dropdown_menu.js');
      $page->register_style($theme->stylePath("menu.css"));
      
      $atkmenutop = $ATK_VARS["atkmenutop"];
      if($atkmenutop=="") $atkmenutop="main";

      $menu = "<div id='nav'>";
      $menu.=$this->getHeader($atkmenutop);

      $menu.="\n".'<ul>';
      foreach ($g_menu[$atkmenutop] as $menuitem)
      {
        $menu .= $this->getMenuItem($menuitem);
      }
      $menu.="<li><a href='app.php?atklogout=1'>".atktext('logout')."</a></li>";
      $menu.='</ul>'."\n";
      
      $menu.=$this->getFooter($atkmenutop);
      $menu.='</div>';
      return $menu;
    }
    
    function getMenuItem($menuitem)
    {
      global $g_menu;
      if (!$menuitem['url'])
      {
        $submenu='<ul>'."\n";
        foreach ($g_menu[$menuitem['name']] as $submenuitem)
        {
          $submenu.=$this->getMenuItem($submenuitem);
        }
        $submenu.='</ul>'."\n";
        $menu.=$this->getItemHtml($menuitem, $submenu);
      }
      else
      {
        $menu.=$this->getItemHtml($menuitem);
      }
      return $menu;
    }
    
    function getItemHtml($menuitem, $submenu="")
    {
      $delimiter = atkconfig("menu_delimiter");
      
      if ($menuitem['name']=='-') return '';
      if ($menuitem['url']) $href = href($menuitem['url'],$this->getMenuTranslation($menuitem['name'],$menuitem['module']));
      else $href = '<a href ="#">'.$this->getMenuTranslation($menuitem['name'],$menuitem['module']).'</a>';
      
      return "<li>".$href.$delimiter.$submenu."</li>\n";
    }
  }

?>
