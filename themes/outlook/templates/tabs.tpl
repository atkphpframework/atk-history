<br><table border="0" cellpadding="0" cellspacing="0" bgcolor="#606060" width="100%" align="center" valign="top">
  <tr>
    <td width="100%" bgcolor="#D8E4F0">
      <table border="0" cellpadding="0" cellspacing="0">
        <tr>
          {section name=i loop=$tabs}
          
          <td valign="bottom">
            <table style="border: 0px solid black; border-left-width:1px;border-right-width:1px;border-top-width:1px" cellspacing="0" cellpadding="5">
              <tr>
                <td {if $tabs[i].selected}{else}onclick="{$tabs[i].link}" style="cursor: pointer; cursor: hand"{/if} height="22" valign="middle" bgcolor="{if $tabs[i].selected}#E0E5F7{else}#2B98F7{/if}" align="center" nowrap>
                  <span class="{if $tabs[i].selected}tab_selected{else}tab{/if}">
                    {$tabs[i].title}
                    </span>
                </td>
              </tr>
            </table>
          </td>
         
          {/section}
          <td valign="bottom" width="100%">
            <table cellspacing="0" cellpadding="0" border="0" width="100%">
              <tr><td bgcolor="#000000" height="1"><img src="{$themedir}images/blank.gif" height="1" width="1"></td></tr>
            </table>
          </td>
        </tr>
      </table>
      <table width="100%" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td bgcolor="#000000" width="1"><img src="{$themedir}images/blank.gif" width="1"></td>
          <td bgcolor="#E0E5F7" align="left" class="block">
            <table border="0" cellspacing="5" cellpadding="5">
              <tr>          
                <td>
                  {$content}
                </td>
              </tr>
            </table>
          </td>
          <td bgcolor="#000000" width="1"><img src="{$themedir}images/blank.gif" width="1"></td>
        </tr>
        <tr>
          <td bgcolor="#000000" colspan="3" valign="top"><img src="{$themedir}images/blank.gif" height="1" width="1"></td>
        </tr>
      </table>
    </td>
  </tr>
</table>