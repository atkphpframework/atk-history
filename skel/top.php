<?php


  $config_atkroot = "./";
  include_once("atk.inc"); 
  atksession();
  atksecure();
   
  
  $g_layout->initGui();
  $g_layout->output("<html>");
  $g_layout->head($txt_app_title);
  $g_layout->body();
  $g_layout->ui_top("Top Frame");
  
  $loggedin = "Ingelogde gebruiker: <b>".$g_user["name"]."</b>";
  
  $g_layout->output('<br>'.$loggedin.' &nbsp; <a href="app.php?atklogout=1" target="_top">Uitloggen</a><br>&nbsp;');
  $g_layout->ui_bottom();
  $g_layout->outputFlush();
?>
