<?php

  class atkUI
  {
    var $m_smarty = NULL;  
    var $m_theme = NULL;
    var $m_node = NULL;
    
    function atkUI()
    {
      atkimport("atk.ui.atktheme");      
      $this->m_theme = &atkTheme::getInstance();
      
      atkimport("atk.ui.atksmarty");
      $this->m_smarty = &atkSmarty::getInstance();          
      
      $this->m_smarty->assign("themedir", $this->m_theme->themeDir());

      $this->m_smarty->assign("atkroot", atkconfig("atkroot"));
      atkdebug("Created a new atkUI");            
    }        
    
    function setNode(&$node)
    {
      $this->m_node = &$node;
    }
    
    function renderAction($action, $vars)
    {
      // todo.. action specific templates
      return $this->render("action.tpl", $vars);
    }      
    
    function renderList($action, $vars)
    {
      return $this->render("list.tpl", $vars);
    }
    
    function renderBox($vars)
    {
      return $this->render("box.tpl", $vars);
    }
    
    function renderTabs($vars)
    {
      return $this->render("tabs.tpl", $vars);
    }
    
    function render($tplname, $vars=array())
    { 
      // Backwardscompatibility hack.. g_layout must be initGui'd
      global $g_layout;
      $g_layout->initGui();

      $this->m_smarty->assign($vars);              
      
      // if $tplname contains a full pathname, the template
      // is rendered as is. 
      // If it's just a filename, we lookup the template in
      // the theme directory.
      if (strpos($tplname, "/")===false)
      {
        // lookup template in theme.
        $tplname = $this->m_theme->tplPath($tplname);
      }
                 
      // Default theme assigns.               
      return $this->m_smarty->fetch($tplname);
      
    }        
    
   /**
     * This function returns a suitable title text for an action.
     * Example: echo $g_layout->title("employee", "edit"); might return:
     *          'Edit an existing employee'
     */
    function title($module, $nodetype, $action)
    {
      // first look for specific text.
      $key = 'title_'.$module.'_'.$nodetype.'_'.$action;
      if ($GLOBALS["txt_".$key]!="")
      {
        return text($key);
      }
      else
      {
        $key = $module."_".$nodetype;
        if ($GLOBALS["txt_".$key]!="")
        {
          return text($key)." - ".text($action, $nodetype);
        }
        else
        {
          $key = 'title_'.$nodetype.'_'.$action;
          if ($GLOBALS["txt_".$key]!="")
          {
            return text($key);
          }
          else
          {
            // if no specific text available for this node,
            // generate a generic title
            return text($nodetype, $module)." - ".text($action, $nodetype);
          }
        }
      }
    }
      
  }

?>
