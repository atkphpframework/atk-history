<?php
  $page = &atknew("atk.ui.atkpage");
  $ui =   &atknew("atk.ui.atkui");
?>

<HTML>
<HEAD>
<TITLE>menu</TITLE>
<LINK HREF="../../platform/atk/style.php" rel="stylesheet" type="text/css">
<LINK REL="stylesheet" HREF="<?php echo $ui->stylePath("dhtml_menu.css") ?>">
 
</HEAD>
<BODY>
<TABLE style="border: 1px solid black" width="100%" cellpadding="0" cellspacing="0">
<TR>
<TD style="border: 0px solid black; border-bottom-width: 1px" bgcolor="#2B98F7" height="26"><div align="center">
<small style="color:#FFFFFF;font-size:11pt;font-family: Verdana,Helvetica,Arial"><b><?php echo strtoupper($product_name); ?></b></small></div>
</TD>
</TR>
<TR>
<TD bgcolor='#D6DDF7' height="22"><div align="left" style="margin-left: 15px">
<small style="color:#666666;font-size:8pt;font-family: Verdana,Helvetica,Arial">Logged in as: <?php echo $g_user["name"]; ?></small></div>
</TD>
</TR>
</TABLE>
<script language="JavaScript" src="<?php echo $config_atkroot; ?>javascript/dhtml_menu_events.js" type="text/javascript">

</BODY>
</HTML>
