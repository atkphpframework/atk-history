<?php
  $config_atkroot = "./";  
  include_once("atk.inc");

  if (atkconfig("fullscreen"))
  {
    // Fullscreen mode. Use index.php as launcher, and launch app.php fullscreen. 
    atksession();
    atksecure();

    $g_layout->register_script(atkconfig("atkroot")."atk/javascript/launcher.js");
    $g_layout->initGui();
    $g_layout->ui_top(text('app_launcher'));
    $g_layout->output('<script language="javascript">atkLaunchApp(); </script>');
    $g_layout->output('<br><br><a href="#" onClick="atkLaunchApp()">'.text('app_reopen').'</a> &nbsp; '.
                      '<a href="#" onClick="window.close()">'.text('app_close').'</a><br><br>');
    $g_layout->ui_bottom();
    $g_layout->page();
    $g_layout->outputFlush();
  }
  else
  {
    // Regular mode. app.php can be included directly.
    include "app.php";
  }
 
?>
