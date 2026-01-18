# Building a Simple Application

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

Even the simplest application that presents the user with a graphic interface represents a staggering amount of programming effort—or so it would seem. An application with even a few menus and windows contains tens of thousands of lines of source code. Yet the application kit relieves you of the responsibility of creating most of this code yourself. Instead, you create your application by programming objects supplied by the application kit, and by plugging them together in Interface Builder.

This chapter outlines the steps necessary to get a "simple" application up and running. By that, we mean an application that provides the user with an interface without requiring anything but the most rudimentary amount of programming. Such a simple application can have menus, windows, text fields, buttons, and a variety of other user-interface devices; however, it does very little in response to user actions beyond providing information and causing trivial changes to the state of the application.

---

## Creating a Project with Project Builder

A *project* is a collection of source files, resource files, and configuration information. By convention, the project files are stored in a single directory. The application's code and much of the user interface are created by editing and compiling source files in the project. Each source file can be edited with your choice of text editor.

The first step in creating an application is to create a project directory and enter the appropriate files into it. Creating a project is greatly facilitated by an application called Project Builder, which is located in the `/NextDeveloper/Apps` directory (and linked at `/NextApps/ProjectBuilder.app`). See Chapter 2, "The Project Builder Application," for complete information on Project Builder.

Project Builder has the following functions:

- Creates a new project directory with appropriate `Makefile`s, source files, and Interface Builder files
- Manages the files in a project
- Provides an interface to the compiler so you can make and debug a project
- Allows you to edit and browse source files
- Provides an interface to debugging and profiling software

To start Project Builder, open it from the Workspace Manager's File Viewer, or type:

```bash
open /NextApps/ProjectBuilder.app
```

at the shell prompt. To create a new project, choose the **New** command in Project Builder's **Project** menu. In the New Project panel, supply a name and choose a project type. Click **OK** and the project directory is created and the default project files appear in the Files View of Project Builder's main window.

---

## Creating the User Interface with Interface Builder

Interface Builder (located in `/NextDeveloper/Apps` and linked at `/NextApps/InterfaceBuilder.app`) is used to create a user interface for an application. You design your application's interface by arranging ready-made user-interface objects—such as menus, windows, and buttons—on the screen.

In addition to laying out your application's user interface graphically, Interface Builder generates the code needed to create these objects and to maintain a reference to them at run time. These instructions, the application's interface objects, and all the information necessary to re-create them in Interface Builder are saved in an *interface file*, or *nib file* (a nib file is also called a *nib*, and the extension of the file's name is `.nib`).

At run time, when the program containing your code is executed, the interface objects are automatically created as the nib file is read into the program. Only the objects that present or control your application's user interface should be created in Interface Builder. Other objects—those that do work for the application—should be instantiated through conventional programming methods.

Interface Builder has two main functions:

- **Lay out the user interface:** Windows, menus, and controls are selected from a palette of standard objects and laid out on the screen exactly as they should appear when the application is running.

- **Make connections to your code:** When the user manipulates your application's windows and controls, methods that you have written in the application's source code must be invoked to perform the appropriate actions. Connections can also be made in the opposite direction, so that the state of an object (such as a text field) can be programmatically changed and displayed to the user.

When you create a new project using Project Builder, an Interface Builder file is created for you automatically. For an application, this file is opened by double-clicking the application's nib file icon in Project Builder's Files View. See Chapter 3, "The Interface Builder Application," for complete information on Interface Builder.

---

## Testing the Interface

The initial user interface for a project already has functional menus and a few of the simpler features of a finished application. An application project is complete enough to compile and run, even before a single line of code is changed or added. Before compiling and running, however, you can test the interface by choosing the **Test Interface** command in Interface Builder's **Document** menu. This command brings the windows and menus on the screen to life—you can open and close menus, click buttons, and type in text fields.

---

## Preparing to Compile

When you have completed editing your interface file and your source files, the next step is to compile the project into a working application. This is done using the `make` application, which Project Builder runs for you.

But first, while still in Interface Builder, choose **Unparse** from the **Classes** submenu of the **Document** menu. This creates the header file (`.h`) and method file (`.m`) for any custom classes that you have defined in Interface Builder. It also updates any existing header and method files for custom classes that have been changed since they were last unparsed.

Then return to Project Builder, either by choosing **Project Builder** from Interface Builder's **Tools** menu or by clicking the Project Builder window. Make sure that all changes have been saved from any open edit windows in your text editor. To open an edit window, double-click a file name in the Files View in Project Builder.

---

## Running the Application

In Project Builder, use the **Make** command in the **Build** menu to compile the files in the project. Error messages and warning are displayed in Project Builder's main window.

After compiling successfully, your application is built and is ready to run. You can run the application by clicking the **Run** button in the lower left of Project Builder's main window, or by choosing **Run** from the **Build** menu.

If you run the application before fixing all errors, you'll get a panel that lists any errors and warns you that the program is out of date. The program, if it can be found, will run anyway using the last successfully compiled version.

If there were no errors in compiling, the application will be opened by the Workspace Manager. An icon for the application will appear in your dock if the application is not set to be automatically launched.
