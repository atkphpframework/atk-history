<?php

  class atkUI
  {
    var $m_smarty = NULL;  
    var $m_theme = NULL;
    
    function atkUI()
    {
      atkimport("atk.ui.atktheme");      
      $this->m_theme = &atkTheme::getInstance();
      
      atkimport("atk.ui.atksmarty");
      $this->m_smarty = &atkSmarty::getInstance();          
      
      $this->m_smarty->assign("themedir", $this->m_theme->themeDir());
            
    }        
    
    function render($tplname, $vars=array())
    { 
      // Backwardscompatibility hack.. g_layout must be initGui'd
      global $g_layout;
      $g_layout->initGui();

      $this->m_smarty->assign($vars);              
           
      // Default theme assigns.               
      return $this->m_smarty->fetch($this->m_theme->tplPath($tplname));
      
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