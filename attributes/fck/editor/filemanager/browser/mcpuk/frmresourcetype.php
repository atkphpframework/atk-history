<!--
 * FCKeditor - The text editor for internet
 * Copyright (C) 2003-2005 Frederico Caldeira Knabben
 * 
 * Licensed under the terms of the GNU Lesser General Public License:
 * 		http://www.opensource.org/licenses/lgpl-license.php
 * 
 * For further information visit:
 * 		http://www.fckeditor.net/
 * 
 * File Name: frmresourcetype.html
 * 	This page shows the list of available resource types.
 * 
 * File Authors:
 * 		Frederico Caldeira Knabben (fredck@fckeditor.net)
-->
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<meta name="vs_targetSchema" content="http://schemas.microsoft.com/intellisense/ie5">
		<link href="<? echo $_GET["themecss"]; ?>" type="text/css" rel="stylesheet">
		<script type="text/javascript" src="js/common.js"></script>
		<script language="javascript">

function SetResourceType( type )
{
	window.parent.frames["frmFolders"].SetResourceType( type ) ;
}

var aTypes = [
	['File','File'],
	['Image','Image'],
	['Flash','Flash'],
	['Media','Media']
] ;

window.onload = function()
{
   var bHasType = ( oConnector.ResourceType.length > 0 ) ;
   cmbTypeObj=document.getElementById("cmbType");
   for ( var i = 0 ; i < aTypes.length ; i++ )
   {
       if ( !bHasType || aTypes[i][0] ==
oConnector.ResourceType )
           AddSelectOption( cmbTypeObj, aTypes[i][1],
aTypes[i][0] ) ;
   }
} 

		</script>
	</head>
	<body bottomMargin="0" topMargin="0">
		<table height="100%" cellSpacing="0" cellPadding="0" width="100%" border="0">
			<tr>
				<td nowrap>
					Resource Type<BR>
					<select id="cmbType" style="WIDTH: 100%" onchange="SetResourceType(this.value);">
					</select>
				</td>
			</tr>
		</table>
	</body>
</html>
