*** Settings ***
Library   String
Library   SerialLibrary

*** Variables ***
${com}   	COM3
${baud} 	115200
${board}	nRF


*** Test Cases ***
Connect Serial
	Log To Console  Connecting to ${board}
	Add Port  ${com}  baudrate=${baud}  encoding=ascii
	Port Should Be Open  ${com}
	Reset Input Buffer
	Reset Output Buffer

Valid Input
	# lähetetään RYG ja lopetusmerkki X
	Write Data   000120X   encoding=ascii
	Log To Console   Send sequence 000120X

	# vastaanotetaan merkkijono kunnes lopetusmerkki X (58) 
	${read} =   Read Until   terminator=58   encoding=ascii 

	# konsolille näkyviin vastaanotettu merkkijono
	Log To Console   Received ${read}
	
	# vertaillaan merkkijonoa
	Should Be Equal As Strings   ${read}    80X
	Log To Console   Tested ${read} is same as 80X

	# astetta hankalampi tehdä testaus numeroina
	# koska lopetusmerkki X pitää ensin poistaa merkkijonosta
	# tai vaihtaa lopetusmerkki esim \0
	# Should Be Equal As Integers   ${read}    -1

Valid Input 2
	Write Data   010059X   encoding=ascii
	Log To Console   Send sequence 010059X

	${read} =   Read Until   terminator=58   encoding=ascii 

	Log To Console   Received ${read}
	
	Should Be Equal As Strings   ${read}    3659X
	Log To Console   Tested ${read} is same as 3659X


Invalid Time Input
	Write Data   000160X   encoding=ascii
	Log To Console   Send sequence 000160X
	${read} =   Read Until   terminator=58   encoding=ascii
	Log To Console   Received ${read}
	Should Be Equal As Strings   ${read}    -3X
	Log To Console   Tested ${read} is same as -3X

Invalid Length Input
	Write Data   60X   encoding=ascii
	Log To Console   Send sequence aX
	${read} =   Read Until   terminator=58   encoding=ascii
	Log To Console   Received ${read}
	Should Be Equal As Strings   ${read}    -1X
	Log To Console   Tested ${read} is same as -1X

Invalid Character Input
	Write Data   000a20X   encoding=ascii
	Log To Console   Send sequence aX
	${read} =   Read Until   terminator=58   encoding=ascii
	Log To Console   Received ${read}
	Should Be Equal As Strings   ${read}    -4X
	Log To Console   Tested ${read} is same as -4X



Disconnect Serial
	Log To Console  Disconnecting ${board}
	[TearDown]  Delete Port  ${com}














	
	
	

