<table width="100%">
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
    <tr>
      {if $field.line!=""}
        <td colspan="2" valign="top">{$field.line}</td>      
      {else}
        <td valign="top" class="{if $field.error!=""}error{else}fieldlabel{/if}">{if $field.label!=""}{$field.label}: {/if}</td>
        <td valign="top" class="field">{$field.full}</td>
      {/if}
    </tr>
  {/foreach}
</table>