<br>
<table id="editform" width="100%">
  {if (count($errors)>0)}
    <tr>
      <td colspan="2" class="error">
        {$errortitle}
        {foreach from=$errors item=error}
          <br>{$error.label}: {$error.msg} {if $error.tab!=""} ({$error.tab}){/if}          
        {/foreach}
      </td>
    </tr>
  {/if}  
  {foreach from=$fields item=field}
    <tr{if $field.rowid != ""} id="{$field.rowid}"{/if}{if $field.initial_on_tab!='yes'} style="display: none"{/if} class="{$field.tab}">
      {if $field.line!=""}
        <td colspan="2" valign="top">{$field.line}</td>      
      {else}
      {if $field.label!=="AF_NO_LABEL"}<td valign="top" class="{if $field.error!=""}errorlabel{else}fieldlabel{/if}">{if $field.label!=""}{$field.label}: {/if}</td>{/if}
        <td valign="top" {if $field.label==="AF_NO_LABEL"}colspan="2"{/if} class="field">{$field.full}</td>
      {/if}
    </tr>
  {/foreach}
</table>