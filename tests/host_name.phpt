--TEST--
asyscon.host-name setting
--INI--
asyscon.host-name = SERVER_NAME
--FILE--
<?php
echo "Server Name: " . SERVER_NAME . "\n";
?>
--EXPECTREGEX--
Server Name: [a-zA-Z_][a-zA-Z0-9_]*
