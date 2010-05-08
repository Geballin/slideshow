-- MNSlideShowX.applescript
-- MNSlideShowX

--  Created by G. Ballin
--  Copyright 2010 MacNiaque. All rights reserved.

on awake from nib theObject
	if theObject is "Window1" then
		set theObject to frontmost
	end if
end awake from nib

on clicked theObject
	if the name of theObject is "ShowAdvanced" then
		set etat to state of drawer "Tiroir1" of window "window1" -- check if open or close
		if (etat is equal to drawer closed) or (etat is equal to drawer closing) then
			tell drawer "Tiroir1" of window "window1" to open drawer
		else
			tell drawer "Tiroir1" of window "window1" to close drawer
		end if
	end if
	if the name of theObject is "BrowseBut" then
		set pictFolder to choose folder with prompt "Select a picture folder"
		set pictFolder to (pictFolder as text)
		set content of text field "folderPathTF" of window "window1" to pictFolder
	end if
	if the name of theObject is "LaunchBut" then
		if contents of text field "folderPathTF" of window "window1" is "" then
			display dialog "Please select a picture folder first." buttons {"OK"} default button 1
			return
		end if
		set slideshowCommand to "\"" & (POSIX path of (path to resource "Slideshow.app") as text) & "Contents/MacOS/Slideshow\" "
		if state of button "runFS_box" of drawer "Tiroir1" of window "window1" is 0 then
			set slideshowCommand to slideshowCommand & "-window "
		end if
		if state of button "dispNbr_box" of drawer "Tiroir1" of window "window1" is 1 then
			set slideshowCommand to slideshowCommand & "-number "
		end if
		if state of button "dispFN_box" of drawer "Tiroir1" of window "window1" is 1 then
			set slideshowCommand to slideshowCommand & "-name "
		end if
		if state of button "repeat_box" of drawer "Tiroir1" of window "window1" is 1 then
			set slideshowCommand to slideshowCommand & "-repeat "
		end if
		if state of button "recurse_box" of drawer "Tiroir1" of window "window1" is 1 then
			set slideshowCommand to slideshowCommand & "-recurse "
		end if
		if state of button "randomize_box" of drawer "Tiroir1" of window "window1" is 1 then
			set slideshowCommand to slideshowCommand & "-random "
		else
			if state of button "sort_choice" of drawer "Tiroir1" of window "window1" is 1 then
				set slideshowCommand to slideshowCommand & "-sort n+ "
			else if state of button "sort_choice" of drawer "Tiroir1" of window "window1" is 2 then
				set slideshowCommand to slideshowCommand & "-sort n- "
			else if state of button "sort_choice" of drawer "Tiroir1" of window "window1" is 3 then
				set slideshowCommand to slideshowCommand & "-sort cd+ "
			else if state of button "sort_choice" of drawer "Tiroir1" of window "window1" is 4 then
				set slideshowCommand to slideshowCommand & "-sort cd- "
			else if state of button "sort_choice" of drawer "Tiroir1" of window "window1" is 5 then
				set slideshowCommand to slideshowCommand & "-sort md+ "
			else if state of button "sort_choice" of drawer "Tiroir1" of window "window1" is 6 then
				set slideshowCommand to slideshowCommand & "-sort md- "
			end if
		end if
		set slideshowCommand to slideshowCommand & "-advance " & (contents of text field "advance_field" of drawer "Tiroir1" of window "window1" as text) & " "
		if state of button "transition_choice" of drawer "Tiroir1" of window "window1" is 1 then
			set slideshowCommand to slideshowCommand & "-tran random "
		else if state of button "transition_choice" of drawer "Tiroir1" of window "window1" is 2 then
			set slideshowCommand to slideshowCommand & "-tran fade "
		else if state of button "transition_choice" of drawer "Tiroir1" of window "window1" is 3 then
			set slideshowCommand to slideshowCommand & "-tran checkers "
		else if state of button "transition_choice" of drawer "Tiroir1" of window "window1" is 4 then
			set slideshowCommand to slideshowCommand & "-tran blinds "
		else if state of button "transition_choice" of drawer "Tiroir1" of window "window1" is 5 then
			set slideshowCommand to slideshowCommand & "-tran uncover "
		end if
		set slideshowCommand to slideshowCommand & "\"" & (POSIX path of (contents of text field "folderPathTF" of window "window1" as text)) & "\""
		--		display dialog slideshowCommand
		do shell script slideshowCommand
	end if
end clicked

on should quit after last window closed theObject
	return true
end should quit after last window closed

on should close theObject
	if the name of theObject is "Tiroir1" then
		set state of button "ShowAdvanced" of window "window1" to "Selected"
	end if
	return true
end should close