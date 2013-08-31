QBinder
=======

QBinder - An open-source Windows keybinder, coded in C/++, using the WINAPI

Version: 1.1.0

Features:

    Projects:
        You can now save/load projects that contain your keybinds/keybind details

    Multi-language:
        Multiple language support (Work in progress)

    Preferences:
        You can enable/disable features according to your preferences

    Customisation:
        You can customise your application's appearance

    Pages:
        The use of pages makes the amount of keybinds you can have virtually unlimited

    Autocomplete:
        Added an autocomplete feature - A window shows all the tokens and descriptions on the go, when you
        press %

    Tokens:
  
        Key Tokens:
            Each of these tokens simulates a key being pressed 
                
            %e - Represents the Return key
            %s - Represents the Shift key
            %b - Represents the Backspace key
            %t - Represents the Tab key
            %c - Represents the Control key
            %a - Represents the Menu/Alt key
            %l - Represents the Caps Lock key
            %p - Represents the Escape key
            %n - Represents the End key
            %u - Represents the Page Up key
            %d - Represents the Page Down key
            %h - Represents the Home key
            %i - Represents the Insert key
            %U - Represents the Up Arrow key
            %D - Represents the Down Arrow key
            %L - Represents the Left Arrow key
            %R - Represents the Right Arrow key
      
        Hold Tokens:
            Each of these tokens simulates a key being held
            
            %=s - Start holding the Shift key
            %=c - Start holding the Control key
            %=a - Start holding the Menu/Alt key
            
            %!s - Stop holding the Shift key
            %!c - Stop holding the Control key
            %!a - Stop holding the Menu/Alt key
        
        Misc. Tokens:
            A.K.A Macros
            
            %r - Simulates a random key press
            %xXX - Where XX is hexadecimal - Presses the key with virtual key code XX (For more information visit http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx)
            %fXX - Where XX is a decimal number (1-24) - Presses a Function key (F1-F24)
            %T - Echoes the current local time - Format: hh:mm:ss
            %J - Echoes the current local date - Format: dd/mm/yyy
      
