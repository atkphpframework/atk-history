
  function mto_parse(link, value)
  {
     var value_array = value.split('=');
     if(value_array[1]=='') return -1;
    
     var atkselector = value.replace("='", "_1253D_12527").replace("'", "_12527");
     return link.replace('REPLACEME', atkselector);
  }
