<?php
  /**
   * The password changer, for the forgetfull
   * 
   * Possible additional features, to be implemented when deemed necessary:
   * - Adding a link in the mail that would automatically notify someone 
   *   if the user did not request a password change
   * - Automatically sending a mail to the same someone of above that a password
   *   has been changed by someone with IP adress xx.xxx.xxx.xxx and more data of the changer
   * - More aesthetically pleasing layout.
   *
   * @author Boy Baukema <boy@ibuildings.nl>
   * @version $Revision$
   */
  $config_platformroot = "../";
  $config_atkroot = "../";
  require_once($config_platformroot."atk.inc");
  $authClass = "auth_".atkconfig("authentication");
  include_once(atkconfig("atkroot")."atk/security/class.".$authClass.".inc");
  
  atksession("passchange");
  global $ATK_VARS, $g_db;
  parse_str($_SERVER['QUERY_STRING'],$URLvars);
  
  $email = $ATK_VARS["auth_email"];
  if($URLvars['id2']) $id2=$URLvars['id2'];
  else $id2 = $ATK_VARS["auth_id"];
  $location = "http://".$_SERVER['SERVER_NAME'].$PHP_SELF;
  $loginattempts = sessionLoad('loginattempts');
  $obj = new $authClass();
  $passw1 = $ATK_VARS["auth_passw1"];
  $passw2 = $ATK_VARS["auth_passw2"];
  $page = &atknew("atk.ui.atkpage");
  $ui = &atknew("atk.ui.atkui");
  if($URLvars['user']) $user=$URLvars['user'];
  else $user = $ATK_VARS["auth_user"];
  $userinfo = $obj->getUser($user);
  
  if($loginattempts=="") $loginattempts = 0;
      
  if($loginattempts=="") 
  {        
    $loginattempts = 1;
  }
  else
  {     
   $loginattempts++;
  }
  
  atkdebug('LoginAttempts: '.$loginattempts);
  sessionStore('loginattempts', $loginattempts);  

  $output.='<form action="'.$_SERVER["PHP_SELF"].'" method="post">';
  $output.= makeHiddenPostVars(array("loginattempts"));
  $output.='<table bgcolor=#D8E4F0>';
  
  // max_loginattempts of 0 means no maximum.
  if(atkconfig('max_loginattempts')>0 && $loginattempts>atkconfig('max_loginattempts'))
  {
    $output.="<tr><td class=table>".text('auth_max_loginattempts_exceeded')."<br><br></td></tr>";
  }
  else if ($id2 AND $passw1 AND $passw2)
  {
    atkdebug("id2 and passwords");
    if ($passw1==$passw2)
    {
      if ($userinfo["id"] == $id2)
      {
	$g_db->query("UPDATE user_user SET password = '".md5($passw1)."' WHERE userid = '$user'");
	header("Location: index.php");
      }
      else{sessionStore('err', text(passchange_id_error));}
    }
    else{sessionStore('err', text(passchange_password_error));}
  }
  else if ($id2 AND $user)
  {
    $output.="<input type='hidden' name='auth_user' value=$user>";
    $output.="<tr><td valign=top class=table>".text('id').":</td><td><input type=\"text\" size=\"15\" name=\"auth_id\" value=\"$id2\"></td></tr>\n";
    $output.="<tr><td valign=top class=table>".text('password').":</td><td><input type=\"password\" size=\"15\" name=\"auth_passw1\"></td></tr>\n";
    $output.="<tr><td valign=top class=table>".text('password').":</td><td><input type=\"password\" size=\"15\" name=\"auth_passw2\"></td></tr>\n";
    $output.="<tr><td colspan=2 class=table height=6></td></tr>\n";
    $output.="<tr><td class=table colspan=2 align=center height=50 valign=middle><input type=submit value=\"Submit\"></td></tr>\n";
    $text=text(password_change2);
    sessionStore('err', null);
  }
  else if ($user AND $email)
  {
    if ($userinfo["email"])
    {
      if ($email == $userinfo["email"])
      {
        $id1 = rand(500,20000);
        $id1 = md5($id1);
	$id1 = substr($id1, 0, 15);
	sessionStore('loginattempts', 0);
	$g_db->query("UPDATE user_user SET id = '$id1' WHERE userid = '$user'");
        mail($email, text(passchange_subject_mail), sprintf(text(passchange_body_mail),$id1,$location,$user,$id1));
        $output.="<tr><td valign=top class=table>".text('id').":</td><td><input type=\"text\" size=\"15\" name=\"auth_id\" value=\"$id2\"></td></tr>\n";
        $output.="<tr><td valign=top class=table>".text('password').":</td><td><input type=\"password\" size=\"15\" name=\"auth_passw1\"></td></tr>\n";
        $output.="<tr><td valign=top class=table>".text('password').":</td><td><input type=\"password\" size=\"15\" name=\"auth_passw2\"></td></tr>\n";
        $output.="<tr><td colspan=2 class=table height=6></td></tr>\n";
        $output.="<tr><td class=table colspan=2 align=center height=50 valign=middle><input type=submit value=\"Submit\"></td></tr>\n";
        $text=text(password_change2);
        sessionStore('err', null);
      }
      else
      {sessionStore('err', text(passchange_email_error));}
    }
    else{sessionStore('err', text(passchange_email_error));}
  }
  else
  {
    $output.="<tr><td valign=top class=table>".text('username').":</td><td><input type=\"text\" size=\"15\" name=\"auth_user\"></td></tr>\n";
    $output.="<tr><td valign=top class=table>".text('email').":</td><td><input type=\"text\" size=\"15\" name=\"auth_email\"></td></tr>\n";
    $output.="<tr><td colspan=2 class=table height=6></td></tr>\n";
    $output.="<tr><td class=table colspan=2 align=center height=50 valign=middle><input type=submit value=\"Submit\"></td></tr>\n";
    $text=text(password_change1);
    sessionStore('err', null);
  }
  $output.='</table></form>';
  
  if (sessionLoad('err'))
  {
    $output.='<font color="#ff0000"><b>'.sessionLoad('err').'</b></font>';
    $output.='<br><a href="javascript:history.back()"><font color=#ff0000>'.text(passchange_try_again).'</font></a>';
    sessionStore('err', null);
  }
  
  $page->addContent($ui->render("passchange.tpl", array("title"=>text(passchange_title), "content"=>$output, "text"=>$text)));       
  $page = $page->render($product_name);
  
  $output = &atkOutput::getInstance();
  $output->output($page);
  $output->outputFlush();
?>
