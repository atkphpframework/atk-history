<?php
  $config_atkroot = "./";
  include_once("atk.inc"); 
  
  $page = &atknew("atk.ui.atkpage");  
  $ui = &atknew("atk.ui.atkui");  
  $theme = &atkTheme::getInstance();
  $output = &atkOutput::getInstance();
  
  $page->register_style($theme->stylePath("style.css"));
  
  $box = $ui->renderBox(array("title"=>text("topframe"),
                                            "content"=>"<br>This is your first top frame<br>&nbsp;"));
 
  $page->addContent($box);

  $output->output($page->render(text('txt_app_title'), true));
  
  $output->outputFlush();
?>
