/*------------------------------------------------------------------------------
-- SOURCE FILE:	signal_handler.c - Manages all GUI events
-- 
-- PROGRAM:     chat
-- 
-- FUNCTIONS:   on_btn_client_activate(GtkWidget *window_client)
--              on_btn_server_activate(GtkWidget *window_server)
--              on_help_about_activate(GtkWidget *dialogue_about)
--              on_server_file_listen_activate()
--              on_server_view_clear_activate()
--              on_file_connect_activate(GtkWidget *dialogue_client_connect)
--              on_btn_connect_activate()
--              on_client_view_clear_activate()
--              on_btn_clear_activate()
--              on_btn_send_activate()
--              on_file_save_log_activate()
--              on_file_quit_activate()
--              on_window_destroy()
--              
--              
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- NOTES: This deals with all events generated by the GTK+2.0 GUI.
--
------------------------------------------------------------------------------*/

#include "signal_handler.h"
#include "client.h"
#include "server.h"

/*------------------------------------------------------------------------------
-- FUNCTION:    on_btn_client_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_btn_client_activate(GtkWidget *window_client)
--                   window_client: pointer to the client window
-- 
-- RETURNS: void
-- 
-- NOTES: This function causes the client window to open and hide the main
--        window.
--		  
------------------------------------------------------------------------------*/
void on_btn_client_activate(GtkWidget *window_client) {
	gtk_widget_hide(window_main);
	gtk_widget_show(window_client);
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_btn_server_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_btn_server_activate(GtkWidget *window_server)
--                   window_server: pointer to the server window
-- 
-- RETURNS: void
-- 
-- NOTES: This function causes the server window to open and hides the main
--        window.
--		  
------------------------------------------------------------------------------*/
void on_btn_server_activate(GtkWidget *window_server) {
	gtk_widget_hide(window_main);
	gtk_widget_show(window_server);
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_help_about_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_help_about_activate(GtkWidget *dialogue_about)
--                   dialogue_about: pointer to the about box
-- 
-- RETURNS: void
-- 
-- NOTES: This function brings up the about dialogue box.
--		  
------------------------------------------------------------------------------*/
void on_help_about_activate(GtkWidget *dialogue_about) {
	gtk_dialog_run(GTK_DIALOG(dialogue_about));
	gtk_widget_hide(dialogue_about);
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_server_file_listen_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_server_file_listen_activate()
-- 
-- RETURNS: void
-- 
-- NOTES: This function creates a server listening thread.
--		  
------------------------------------------------------------------------------*/
void on_server_file_listen_activate() {
	pthread_t server_thread;

	/* Create the server thread */
	pthread_create(&server_thread, NULL, &start_server, (void *)NULL);
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_server_view_clear_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_server_view_clear_activate()
-- 
-- RETURNS: void
-- 
-- NOTES: This function clears the list of connected clients.
--		  
------------------------------------------------------------------------------*/
void on_server_view_clear_activate() {
	GtkTextBuffer   *buffer;
	GtkTextIter		start, end;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_client_list));
	gtk_text_buffer_get_bounds(buffer, &start, &end);
	
	gtk_text_buffer_delete(buffer, &start, &end);
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_file_connect_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_file_connect_activate(GtkWidget *dialogue_client_connect)
--                   dialogue_client_connect: pointer to the connection dialogue
-- 
-- RETURNS: void
-- 
-- NOTES: This function brings up a connection dialogue box that will initiate
--        a connection to the server when the client presses connect.
--		  
------------------------------------------------------------------------------*/
void on_file_connect_activate(GtkWidget *dialogue_client_connect) {
	gtk_dialog_run(GTK_DIALOG(dialogue_client_connect));
	gtk_widget_hide(dialogue_client_connect);
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_btn_connect_activate()
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   son_btn_connect_activate()
-- 
-- RETURNS: void
-- 
-- NOTES: This function initiates a connection to the client based on what
--        the user entered into the client connection dialogue box.
--		  
------------------------------------------------------------------------------*/
void on_btn_connect_activate() {
	const gchar	*server;
	pthread_t	client_thread;
	int			r;

	server = gtk_entry_get_text(GTK_ENTRY(entry_server));

	if ((r = start_client(server)) == -1) {
		append_text("Error: connection failed\n");
	}
	else { /* start read thread */
		pthread_create(&client_thread, NULL, &client_recv, (void *)NULL);
	}
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_client_view_clear_activate()
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_client_view_clear_activate()
-- 
-- RETURNS: void
-- 
-- NOTES: This function clears the client's chat log.
--		  
------------------------------------------------------------------------------*/
void on_client_view_clear_activate() {
	GtkTextBuffer   *buffer;
	GtkTextIter		start, end;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_chat_out));
	gtk_text_buffer_get_bounds(buffer, &start, &end);
	
	gtk_text_buffer_delete(buffer, &start, &end);
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_btn_clear_activate()
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_btn_clear_activate()
-- 
-- RETURNS: void
-- 
-- NOTES: This function clears any text entered into the outgoing text window.
--		  
------------------------------------------------------------------------------*/
void on_btn_clear_activate() {
	GtkTextBuffer   *buffer;
	GtkTextIter		start, end;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_chat_input));
	gtk_text_buffer_get_bounds(buffer, &start, &end);
	
	gtk_text_buffer_delete(buffer, &start, &end);
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_btn_send_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_btn_send_activate() 
-- 
-- RETURNS: void
-- 
-- NOTES: This function extracts inputted text to be sent to the server and
--        then clears the input window.
--		  
------------------------------------------------------------------------------*/
void on_btn_send_activate() {
	GtkTextBuffer	*buffer;
	GtkTextIter		start, end;
	gchar			*text;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_chat_input));
	gtk_text_buffer_get_bounds(buffer, &start, &end);

	text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

	client_send(text);	

	on_btn_clear_activate();
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_file_save_log_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_file_save_log_activate()
-- 
-- RETURNS: void
-- 
-- NOTES: This function saves any text in the chat log to a file "chat_log.txt"
--        in the current directory.
--		  
------------------------------------------------------------------------------*/
void on_file_save_log_activate() {
	GtkTextBuffer	*buffer;
	GtkTextIter		start, end;
	gchar			*text;
	FILE *ofp;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_chat_out));
	gtk_text_buffer_get_bounds(buffer, &start, &end);

	text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

	ofp = fopen("./chat_log.txt", "w");
	fprintf(ofp, "%s", text);
	fclose(ofp);

	append_text("Chat log saved as \"chat_log.txt\".\n");
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_file_quit_activate
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_file_quit_activate()
-- 
-- RETURNS: void
-- 
-- NOTES: This function closes the application.
--		  
------------------------------------------------------------------------------*/
void on_file_quit_activate() {
	gtk_main_quit();
}

/*------------------------------------------------------------------------------
-- FUNCTION:    on_window_destroy
-- 
-- DATE:        March 18, 2009
-- 
-- REVISIONS:   
-- 
-- DESIGNER:    Steffen L. Norgren
-- 
-- PROGRAMMER:  Steffen L. Norgren
-- 
-- INTERFACE:   on_window_destroy()
-- 
-- RETURNS: void
-- 
-- NOTES: This function closes the application.
--		  
------------------------------------------------------------------------------*/
void on_window_destroy() {
	gtk_main_quit();
}
