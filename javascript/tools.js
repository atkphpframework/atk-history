/**
 * For getting objects but perserving backwards compatibility
 */
function get_object(name)
{
  if (document.getElementById)
  {
    return document.getElementById(name);
  }
  else if (document.all)
  {
    return document.all[name];
  }
  else if (document.layers)
  {
    return document.layers[name];
  }
  return false;
}

/**
 * Toggles the display on an object
 */
function toggleDisplay(name, obj)
{  
  if (obj.style.display=="none")
  {
    obj.style.display="";		  
  }
  else
  {
    obj.style.display="none";
  }
}

/**
 * Transforms the first character of string to uppercase
 * e.g. kittie => Kittie
 */
function ucfirst(stringtt)
{
  return stringtt.charAt(0).toUpperCase()+stringtt.substring(1,stringtt.length)
}

/**
 * Replace an occurrence of a string 
 */
function str_replace(haystack,needle,replace,casesensitive)
{
	if(casesensitive) return(haystack.split(needle)).join(replace);

	needle=needle.toLowerCase();

	var replaced="";
	var needleindex=haystack.toLowerCase().indexOf(needle);
	while(needleindex>-1)
	{
		replaced+=haystack.substring(0,needleindex)+replace;
		haystack=haystack.substring(needleindex+needle.length);
		needleindex=haystack.toLowerCase().indexOf(find);
	}
	return(replaced+haystack);
}