<?php  

  class atkTheme
  {  
    var $m_themedir = "";
    var $m_initialized = false;
    var $m_def = array();
    
    function &getInstance()
    {
      static $s_instance = NULL;
      if ($s_instance == NULL)
      {        
        $s_instance = new atkTheme();
      }   
      return $s_instance;
    }
    
    function atkTheme()
    {
      // initialize
      atkdebug("Created a new atkTheme instance");
      $this->initTheme(atkconfig("defaulttheme"));  
    }
    
    function initTheme($themename)
    {          
      if (!$this->m_initialized)
      {
  
        // First check local theme directory
        $atkroot = atkconfig("atkroot");
        if(file_exists($atkroot."themes/".$themename))
        {
          $this->m_themedir = $atkroot."themes/".$themename."/";
        }
        // else check atk themes directory
        else if (file_exists($atkroot."atk/themes/".$themename))
        {
          $this->m_themedir = $atkroot."atk/themes/".$themename."/";        
        }
        // else fallback to default.
        else
        {
          atkerror("Theme ".$themename." cannot be found, swithing to default.");
          $this->m_themedir = $atkroot."atk/themes/default/";
        }            
        
        // Include layout.inc file (oldstyle, soon to be deprecated)
        if (file_exists($this->m_themedir."layout.inc"))
        {
          global $g_theme;
          include_once($this->m_themedir."layout.inc");
        }
        
        // Include themedef.inc file (newstyle)
        if (file_exists($this->m_themedir."themedef.inc"))
        {
          include_once($this->m_themedir."themedef.inc");
          $this->m_def = $theme;
        }        
        
        $this->m_initialized = true;          
      }
    }
    
    function tplPath($tpl)
    {    
      if (atk_in_array($tpl, $this->m_def["customtpls"]))
      {
        return $this->themeDir()."templates/".$tpl;
      }
      return atkconfig("atkroot")."atk/themes/default/templates/".$tpl;
    }
    
    function themeDir()
    {
      return $this->m_themedir;
    }
  }

?>