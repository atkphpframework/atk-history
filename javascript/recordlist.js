function highlightrow(row, color)
{
  if (typeof(row.style) != 'undefined') 
  {
    row.oldcolor = row.style.backgroundColor;    
    row.style.backgroundColor = color;
  }
}

function resetrow(row)
{  
  row.style.backgroundColor = row.oldcolor;
}

function selectrow(rlId, rownum)
{
  table = document.getElementById(rlId);
  table.listener.setRow(rownum);
}