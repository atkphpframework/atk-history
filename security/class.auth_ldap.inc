<?php
  /**
   * This class does authentication through an ldap server.
   *
   * @author Sandy Pleyte (sandy@achievo.org)
   * @version $Revision$
   *
   * $Id$
   *
   */
   
   class auth_ldap extends auth_interface
   {
     function validateUser($user, $passwd)
     { 
     
       if($ldap = ldap_connect(atkconfig("auth_ldap_host")))
       {
        atkdebug("successful connection to ".atkconfig("auth_ldap_host"));
        if(atkconfig("auth_ldap_bind_tree"))
        {
          if ($bindID = @ldap_bind($ldap, atkconfig("auth_ldap_bind_dn") , atkconfig("auth_ldap_bind_pw")))
          {
            atkdebug("Succesfully bound to ". atkconfig("auth_ldap_bind_dn") . " with id: ".$bindID . " conn_id ". $ldap);
          }   
          else
          {
            atkdebug("<b>Error binding to</b> ".atkconfig("auth_ldap_bind_dn") ." ". atkconfig("auth_ldap_bind_pw"));
            return AUTH_ERROR;
          }  
        }
      
        // find the dn for this uid, the uid is not always in the dn
        $filter = (atkconfig("auth_ldap_search_filter") != "" ? atkconfig("auth_ldap_search_filter") : "uid");
        $sri = ldap_search($ldap, atkconfig("auth_ldap_context"), $filter."=$user");
        $allValues = ldap_get_entries($ldap, $sri);
        if($allValues["count"] > 0)
        {
          // we only care about the first dn
      	  $userDN = $allValues[0]["dn"];
      	  // generate a bogus password to pass if the user doesn't give us one 
	  // this gets around systems that are anonymous search enabled 
          if (empty($passwd)) $passwd = crypt(microtime()); 
      	  // try to bind as the user with user suplied password
      	  if (ldap_bind($ldap,$userDN, $passwd)) return AUTH_SUCCESS;
        }
	atkdebug("LDAP did not successfully authenticate $user");

        // dn not found or password wrong TODO/FIXME: return -1 if dn not found
        return AUTH_MISMATCH;
       }
       else return AUTH_ERROR;
     }
                         
     function canMd5()
     {
       //return atkconfig("authentication_md5"); // ?? Is this correct? can we store passwords as md5 in ldap?
       return false;
     }
   }

?>