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
 * File Name: frmactualfolder.html
 * 	This page shows the actual folder path.
 * 
 * File Authors:
 * 		Frederico Caldeira Knabben (fredck@fckeditor.net)
-->
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
	<head>
		<link href="<? echo $_GET["themecss"]; ?>" type="text/css" rel="stylesheet">
		<script type="text/javascript">

function OnResize()
{
	divName.style.width = "1px" ;
	divName.style.width = tdName.offsetWidth + "px" ;
}

function SetCurrentFolder( resourceType, folderPath )
{
	document.getElementById('tdName').innerHTML = folderPath ;
}

window.onload = function()
{
	window.top.IsLoadedActualFolder = true ;
}

		</script>
	</head>
	<body bottomMargin="0" topMargin="0">
		<table height="100%" cellSpacing="0" cellPadding="0" width="100%" border="0">
			<tr>
				<td>
					<button style="WIDTH: 100%" type="button">
						<table cellSpacing="0" cellPadding="0" width="100%" border="0">
							<tr>
								<td><img height="32" alt="" src="images/FolderOpened32.gif" width="32"></td>
								<td>&nbsp;</td>
								<td id="tdName" width="100%" nowrap class="ActualFolder">/</td>
								<td>&nbsp;</td>
								<td><!--<img height="8" src="images/ButtonArrow.gif" width="12">--></td>
								<td>&nbsp;</td>
							</tr>
						</table>
					</button>
				</td>
			</tr>
		</table>
	</body>
</html>
