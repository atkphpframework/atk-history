<table border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="left">
      <table border="0" cellspacing="0" cellpadding="0" class="backtable">
        <tr>
          <td>
            <table border="0" cellspacing="1" cellpadding="3">
            
              <!-- header -->
              <tr> 
                {foreach from=$header item=col}
                  <td class="tableheader" {$col.htmlattributes}>
                    {if $col.contents != ""}{$col.contents}{else}&nbsp;{/if}                     
                  </td>
                {/foreach}
              </tr>
              
              <!-- search row -->
              {if count($search)}
              <tr>
              {$searchstart}
              {foreach from=$search item=col}
                  <td class="tableheader" {$col.htmlattributes}>
                    {if $col.contents != ""}{$col.contents}{else}&nbsp;{/if}                     
                  </td>
              {/foreach}
              {$searchend}
              </tr>
              {/if}
              
              <!-- records -->
              {$liststart}
              
              {$listend}
              
            </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>