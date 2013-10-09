# JAM827 Advanced Data Models in Cascades #

Does your app need a list to show items? We’ll talk about retrieving, storing, and 
displaying list data. You will learn about the new data management features that 
have been added as part of the Cascades™ 10.2 release, specifically the AsyncDataModel 
class. This type of data model is good at handling large data sets and it uses cache 
functionality that can save start-up time and conserve memory usage. It also provides 
improved management of hierarchical data, such as the common header/details structure 
that’s used in many lists. We’ll also see how to handle remote data in your lists.

Step by step commit of weatherhistory sample app for BBJAM Asia

Summary:
Welcome to the history of weather. This application gives you the possibility
 to view the weather in the past. As in all dealings with weather the accuracy
 should always be subject for scrutiny. Remember the best way to get an accurate
 weather data point is to look out a window and short of a window, call a friend.
 The purpose of the sample app is to show how to handle large data sets that
 are presented in a list and provided over a network. You will need to have a
 weather server set up and running in order for the sample to work.

 You will learn:
 - How to present data in a list using both a AsyncDataModel and a AsyncHeaderDataModel
 - How to present data retrieved over a network in a list
 - How to implement functionality for asking for more data when reaching the end of a list
 - How to handler errors that can occur when talking to a server
 
 Use the sample code to see how to handle networked data, you will have to connect it to 
 your own server.

========================================================================
Requirements:

BlackBerry 10 Native SDK

========================================================================
Running the example:

1. From the Sample apps page, download and extract one of the sample applications.
2. Launch BlackBerry 10 Native SDK, and from the File menu, select Import.
3. Expand General, and select Existing Projects into Workspace. Click Next.
4. Browse to the location where you extracted the sample file, and then click OK.
5. The sample project should display in the Projects section. 
   Click Finish to import the project into your workspace.
6. In the Project Explorer pane, Right-click the project (for example hellocascades) 
   and select Build Project.
7. In the Project Explorer pane, Right-click the project (for example hellocascades) 
   and select Run As > BlackBerry C/C++ Application.
8. The application will now install and launch on your device. If it doesent you might
   have to set up your environment: 
   http://developer.blackberry.com/cascades/documentation/getting_started/setting_up.html
   

