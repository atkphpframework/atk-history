<?php
  $page->register_style("../../platform/atk/style.php");
  $page->register_style($theme->stylePath("dhtml_menu.css"));
  
  $dhtml_menu = '<script language="JavaScript" src="'.$config_atkroot.'javascript/dhtml_menu_events.js" type="text/javascript"></script>';
  
  $dhtml_menu .= '
	<TABLE style="border: 1px solid black" width="100%" cellpadding="0" cellspacing="0">
	<TR>
	<TD style="border: 0px solid black; border-bottom-width: 1px" bgcolor="#2B98F7" height="26"><div align="center">
	<small style="color:#FFFFFF;font-size:11pt;font-family: Verdana,Helvetica,Arial"><b>'.strtoupper($product_name).'</b></small></div>
	</TD>
	</TR>
	<TR>
	<TD bgcolor="#D6DDF7" height="22"><div align="left" style="margin-left: 15px">
	<small style="color:#666666;font-size:8pt;font-family: Verdana,Helvetica,Arial">Logged in as: '.$g_user["name"].'</small></div>
	</TD>
	</TR>
	</TABLE>
  ';
  
  $page->addContent($dhtml_menu);
?>