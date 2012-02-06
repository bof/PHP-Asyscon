--TEST--
asyscon.host-domain setting
--INI--
asyscon.host-domain = SERVER_DOMAIN
--FILE--
<?php
echo "Server Domain: " . SERVER_DOMAIN . "\nOK\n";
?>
--EXPECTREGEX--
Server Domain: .*
OK
