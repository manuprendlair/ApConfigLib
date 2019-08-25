// Creates an admin page on a webserver which allows the user to update the SSID and Password
// Performs basic checks to ensure that the input values are valid

#ifndef _AdminPage_h
#define _AdminPage_h

//Holds the admin webpage in the program memory
const char adminPage[] PROGMEM = 
   // "<html>"
   //   "<head>"
   //     "<style>input {font-size: 1.2em; width: 100%; max-width: 350px; display: block; margin: 5px auto; } </style>"
   //     "<script> function checkValid() { var password1 = document.getElementById('password1'); var password2 = document.getElementById('password2');"
   //     "if (password1.value != password2.value) { password2.setCustomValidity('Passwords must match'); } else { password2.setCustomValidity(''); }} </script>"
   //   "</head>"
   //   "<body>"
   //     "<form id='form' action='/admin' method='post'>"
   //       "<input name='newssid' type='text' minlength='8' maxlength='16' placeholder='New SSID'>"
   //       "<input name='newpassword' id='password1' type='password' minlength='8' maxlength='16' placeholder='New Password'>"
   //       "<input id='password2' type='password' minlength='8' maxlength='16' placeholder='Confirm Password' onchange='checkValid()'>"
   //       "<input type='submit' value='Update'>"
			//"<p>(Space and Underscore are allowed) </p>"
   //     "</form>"
   //   "</body>"
   // "</html>"

"<html>"
	"<head>"
		"<style>"
			"input{ font - size: 1.2em; width: 100 % ; max - width: 350px; display: block; margin: 5px auto; }"
			"p{"
			"margin: 30px auto;"
			"width: 60 % ;"
			"padding: 5px;"
			"border: 3px solid #39D;"
			"text-align: center;}"
		"</style>"
		"<script>"
			"function checkValid() {"
			"var password1 = document.getElementById('password1');"
			"var password2 = document.getElementById('password2');"
			"if (password1.value != password2.value) {"
				"password2.setCustomValidity('Passwords must match');}"
			"else { password2.setCustomValidity('');}}"
		"</script>"
	"</head>"
	"<body>"
		"<form id='form' action='/admin' method='post'>"
			"<input name='newssid' type='text' minlength='8' maxlength='16' placeholder='New SSID'>"
			"<input name='newpassword' id='password1' type='password' minlength='8' maxlength='16' placeholder='New Password'>"
			"<input id='password2' type='password' minlength='8' maxlength='16' placeholder='Confirm Password' onchange='checkValid()'>"
			"<input type='submit' value='Update'>"
			"<p>(Space and Underscore are allowed) </p>"
		"</form>"
	"</body>"
"</html>"
;


 

#endif
    

