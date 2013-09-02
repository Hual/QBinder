QBinder
=======

QBinder - An open-source Windows keybinder, coded in C/++, using the WINAPI

Version: 1.1.0

Features:

    Listens for:
        Key combinations (shift, alt, ctrl);
        Media keys;
        Common keys;

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
        An autocomplete feature - A window shows all the tokens and descriptions on the go, when you
        press %

    Tokens:
  
        Key Tokens:
            Each of these tokens simulates a key being pressed 
                
            %e - Press the Return key
            %s - Press the Left Shift key
            %S - Press the Right Shift key
            %b - Press the Backspace key
            %t - Press the Tab key
            %c - Press the Left Control key
            %o - Press the Right Control key
            %a - Press the Left Menu/Alt key
            %A - Press the Right Menu/Alt key
            %l - Press the Caps Lock key
            %p - Press the Escape key
            %n - Press the End key
            %u - Press the Page Up key
            %d - Press the Page Down key
            %h - Press the Home key
            %i - Press the Insert key
            %U - Press the Up Arrow key
            %D - Press the Down Arrow key
            %L - Press the Left Arrow key
            %R - Press the Right Arrow key
      
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
            %CXXXX - Sleep for XXXX seconds and resume key press emulation
            %% - Press %
