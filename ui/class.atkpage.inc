<?php

  class atkPage 
  {
    var $m_scripts = array();
    var $m_stylesheets = array();
    var $m_content = "";
        
    
    /**
     * Register a javascript for use withing the page
     * @param $file the filename of the external javascript file
     */
    function register_script($file)
    {
      if (!in_array($file, $this->m_scripts)) $this->m_scripts[] = $file;
    }
    
    /**
     * Register a Cascading StyleSheet for use withing the page
     * @param $file the filename of the external stylesheet
     */
    function register_style($file)
    {    
      if (!in_array($file, $this->m_stylesheets)) $this->m_stylesheets[] = $file;
    }
    
    /**
     * Return a HTML header
     * @param $title Titel van het window
     */
    function head($title)
    {
      $res = "<head>\n  <title>$title</title>\n";
      
      // $Name$ is the CVS tag.
      $version = atkversion();
      if ("\$Name$"!="\$"."Name:  $") $version.=" ($Name$)";
      $res.= '  <meta name="atkversion" value="'.$version.'">'."\n";

      for ($i = 0; $i < count($this->m_scripts); $i++)
        $res.='  <script language="javascript" src="'.$this->m_scripts[$i].'"></script>'."\n";

      $res.="<script language=\"javascript\">\n
               function globalSubmit(form)
               {
                 var retval = true;\n";
      for ($i=0; $i < count($this->m_submitscripts); $i++)
      {
        $res.="retval = ".$this->m_submitscripts[$i]."\n";
        $res.="if (retval!=true) return false;\n";
      }
      $res.="return retval;
               }
             </script>\n";

      for ($i = 0; $i < count($this->m_stylesheets); $i++)
        $res.='  <link href="'.$this->m_stylesheets[$i].'" rel="stylesheet" type="text/css">'."\n";

      $res.="</head>\n";
      return $res;
    }
    
    function addContent($content)
    {      
      $this->m_content.=$content;
    }
    
    /**
     * Return a HTML body
     * @param $extraops Not used
     */
    function body($extraprops="")
    {
      return "<body ".$extraprops.">\n";
    }
    
    function render($title)
    {      
      $page = "<html>\n";
      $page.= $this->head($title);
      $page.= $this->body();
      $page.= $this->m_content;
      $page.= "\n</body>\n</html>\n";            
     
      return $page;
    }
    
    function atkPage()
    {
      // Load default stylesheet.
      $this->register_style((file_exists(atkconfig("atkroot")."./style.php") ? "." : atkconfig("atkroot")."atk").'/style.php');  
    }
        
  }

?>