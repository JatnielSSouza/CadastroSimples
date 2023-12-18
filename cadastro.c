#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>


/* void usar_estilo(){
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path( css_provider, "  ", FALSE);
    gtk_style_context_add_provider_for_screen( gdk_screen_get_default(),
                                                                    GTK_STYLE_PROVIDER(css_provider),
                                                                    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
                                                                    } */

GtkBuilder *builder;
GtkWidget *window;
GtkStack *stack;
GtkListStore *modelo_armazenamento;

typedef struct usuario{
    int id;
    char nome[100];
    char email[100];
    struct usuario *proximo;
} user;

int id = 0;
user *cabecalho_user;
user *proximo_user;

void on_main_window_destroy( GtkWidget *window, gpointer data ){

gtk_main_quit();
}

void mensagem( char text[100], char secondary_text[100], char icon_name[100]){

GtkMessageDialog *mensagem_dialogo = gtk_builder_get_object ( builder, "mensagem");

g_object_set(mensagem_dialogo, "text", text, NULL );
g_object_set(mensagem_dialogo, "secondary_text", secondary_text, NULL );
g_object_set(mensagem_dialogo, "icon_name", icon_name, NULL );

gtk_widget_show_all(mensagem_dialogo);
gtk_dialog_run(mensagem_dialogo);
gtk_widget_hide(mensagem_dialogo);


}

void login( char *email, char *senha, bool *lembrar){

    if ((strcmp(email, "admin") == 0) && (strcmp(senha, "admin") == 0) ){
        mensagem("Bem vindo!", "Usuario logado com sucesso!", "gtk-apply");
        //g_print("Usuario logado!");
        gtk_stack_set_visible_child_name( stack, "view_inicial");
        g_object_set(window, "icon_name", "avatar-default-symbolic", NULL );
    }else{
        mensagem("Aviso!", "Email ou senha incorreta!", "dialog-error");
        //g_print("Email ou senha incorreta!");
    }
}

void on_botao_login_clicked( GtkWidget *widget, gpointer data ){

char *email = gtk_entry_get_text(gtk_builder_get_object(builder, "email"));
char *senha = gtk_entry_get_text(gtk_builder_get_object(builder, "senha"));
bool *lembrar = gtk_toggle_button_get_active(gtk_builder_get_object(builder, "lembrar"));
login( email, senha, lembrar);


//g_print(email);
//g_print(senha);
//g_print("%d", lembrar);
}

void on_botao_cadastrar_inicial_clicked( GtkWidget *widget, gpointer data ){

gtk_stack_set_visible_child_name( stack, "view_cadastro");
}

void on_botao_listar_inicial_clicked( GtkWidget *widget, gpointer data ){

gtk_stack_set_visible_child_name( stack, "view_listar");
}

void on_botao_sair_inicial_clicked( GtkWidget *widget, gpointer data ){

gtk_stack_set_visible_child_name( stack, "view_login");
g_object_set(window, "icon_name", "changes-prevent-symbolic", NULL );
}

void on_botao_cad_voltar_clicked( GtkWidget *widget, gpointer data ){

gtk_stack_set_visible_child_name( stack, "view_inicial");
}

void on_botao_listar_voltar_clicked( GtkWidget *widget, gpointer data ){

gtk_stack_set_visible_child_name( stack, "view_inicial");
}

//Parte lógica da aplicação:
void on_botao_cadastrar_clicked( GtkWidget *widget, gpointer data ){

    char *cad_nome = gtk_entry_get_text(gtk_builder_get_object(builder, "cad_nome"));
    char *cad_email = gtk_entry_get_text(gtk_builder_get_object(builder, "cad_email"));

    if (strcmp(cad_nome, " ") == 0){
        mensagem("Aviso!", "Campo 'Nome' obrigatorio!", "dialog-error");
    }else{
        id++;
        proximo_user->id = id;
        strcpy(proximo_user->nome, cad_nome);
        strcpy(proximo_user->email, cad_email);

        g_print("id: %d | nome: %s | email: %s\n",
                 proximo_user->id,
                 proximo_user->nome,
                 proximo_user->email);

        char texto[100];
        g_snprintf(texto, 100, "%s%s%s", "Usuario ", proximo_user->nome, " cadastrado!" );
        mensagem("Aviso!", texto, "dialog-emblem-default" );

        proximo_user->proximo = (user *)malloc(sizeof(user));
        proximo_user = proximo_user->proximo;
    }
}

void on_botao_listar_clicked( GtkWidget *widget, gpointer data ){

    proximo_user->proximo = NULL;
    proximo_user = cabecalho_user;

    GtkTreeIter *iter;
    gtk_list_store_clear(modelo_armazenamento);

    while(proximo_user->proximo != NULL){

                g_print("id: %d | nome: %s | email: %s\n",
                 proximo_user->id,
                 proximo_user->nome,
                 proximo_user->email);

    gtk_list_store_append(modelo_armazenamento, &iter);
    gtk_list_store_set(modelo_armazenamento, &iter,
                                0, proximo_user->id,
                                1, proximo_user->nome,
                                2, proximo_user->email,
                                -1);


    proximo_user = proximo_user->proximo;

    }
}


int main( int argc, char *argv[ ] ){

    cabecalho_user = (user*)malloc(sizeof(user));//(user*) is a typecasted pointer: Significa que o dado é do tipo user e a memória alocada é para esse tipo de dado.
    proximo_user = cabecalho_user;

 gtk_init( &argc, &argv);

builder = gtk_builder_new_from_file( "user-interface.glade ");

gtk_builder_add_callback_symbols(
builder,

"on_botao_login_clicked", G_CALLBACK(on_botao_login_clicked),
"on_main_window_destroy", G_CALLBACK(on_main_window_destroy) ,
"on_botao_cadastrar_inicial_clicked", G_CALLBACK(on_botao_cadastrar_inicial_clicked) ,
"on_botao_listar_inicial_clicked", G_CALLBACK(on_botao_listar_inicial_clicked) ,
"on_botao_sair_inicial_clicked", G_CALLBACK(on_botao_sair_inicial_clicked) ,
"on_botao_cad_voltar_clicked", G_CALLBACK(on_botao_cad_voltar_clicked) ,
"on_botao_cadastrar_clicked", G_CALLBACK(on_botao_cadastrar_clicked) ,
"on_botao_listar_voltar_clicked", G_CALLBACK(on_botao_listar_voltar_clicked) ,
"on_botao_listar_clicked", G_CALLBACK(on_botao_listar_clicked) ,

NULL);

gtk_builder_connect_signals(builder, NULL);

stack = GTK_STACK(gtk_builder_get_object( builder, "stack"));
window = GTK_WIDGET(gtk_builder_get_object( builder, "main_window"));
modelo_armazenamento = GTK_LIST_STORE(gtk_builder_get_object( builder, "liststore1"));

//usar_estilo();
gtk_widget_show_all(window);
gtk_main();
return 0;

free(proximo_user);
free(cabecalho_user);
}
