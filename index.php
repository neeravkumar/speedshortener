<?php
$check=exec("/home/nero/www/redir/redirect ".substr($_SERVER[REQUEST_URI],1));
if($check!="")
{
	header("Location: http://".$check);
	exit(0);
}
?>
