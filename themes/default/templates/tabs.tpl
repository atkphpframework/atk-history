<script language="JavaScript">
var tabs = new Array();
{section name=i loop=$tabs}tabs[tabs.length] = "{$tabs[i].tab}"; {/section}

var tabColor = "#FFFFFF";
var tabBackground = "#2B98F7";
var tabSelectedColor = "#000000";
var tabSelectedBackground = "#E0E5F7";
</script>

<table border="0" cellpadding="0" cellspacing="0" width="100%" align="center" valign="top">
  <tr>
    <td width="100%">
      <table border="1" cellpadding="2" cellspacing="0">
        <tr>                              
          {section name=i loop=$tabs}
          <td id="{$tabs[i].tab}" onclick="showTab('{$tabs[i].tab}')" style="cursor: pointer; cursor: hand; color:{if $tabs[i].selected}#000000{else}#FFFFFF{/if}; background: {if $tabs[i].selected}#E0E5F7{else}#2B98F7{/if}" valign="middle" align="left" nowrap>
            <b>&nbsp;{$tabs[i].title}&nbsp;</b>
          </td>       
          {/section}
        </tr>
      </table>
      <table border="1" cellspacing="0" cellpadding="5" width="100%">
        <tr>
          <td>
            {$content}
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>