#include <lvgl.h>
#include <TFT_eSPI.h>

// #include <demos/lv_demos.h>                                 //!! lvgl/src/demos
// #include <examples/lv_examples.h>                           //!! lvgl/src/examples

#define SCREEN_ROTATION 0                                  // set the screen rotation

void create_main_menu(lv_obj_t * parent);
void create_file_menu(lv_obj_t * parent, lv_obj_t * prev_scr);

lv_obj_t * prev_scr = NULL;

void menu_event_handler(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * list = lv_obj_get_parent(obj);
    const char * txt = lv_list_get_btn_text(list, obj);

    // Создание нового экрана
    lv_obj_t * new_scr = lv_obj_create(NULL);

    if (strcmp(txt, "New File") == 0) {
        // Создаем новое меню или элементы для "New File"
        create_file_menu(new_scr, lv_scr_act());
    } else {
        // Создаем основное меню на новом экране
        create_main_menu(new_scr);
    }

    // Загрузка нового экрана
    lv_scr_load(new_scr);
}

void back_event_handler(lv_event_t * e) {
    // Возвращаемся на предыдущий экран
    if (prev_scr) {
        lv_scr_load(prev_scr);
        prev_scr = NULL;  // Обнуляем, чтобы предотвратить повторный возврат
    }
}

void create_main_menu(lv_obj_t * parent) {
    // Создаем список
    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));  // Устанавливаем размеры списка на весь экран
    lv_obj_align(list, LV_ALIGN_CENTER, 0, 0);

    // Добавляем элементы списка
    lv_obj_t * btn;

    btn = lv_list_add_btn(list, LV_SYMBOL_FILE, "New File");
    lv_obj_add_event_cb(btn, menu_event_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "New Directory");
    lv_obj_add_event_cb(btn, menu_event_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(list, LV_SYMBOL_CLOSE, "Delete");
    lv_obj_add_event_cb(btn, menu_event_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(list, LV_SYMBOL_EDIT, "Edit");
    lv_obj_add_event_cb(btn, menu_event_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(list, LV_SYMBOL_SAVE, "Save");
    lv_obj_add_event_cb(btn, menu_event_handler, LV_EVENT_CLICKED, NULL);

    // Устанавливаем размер списка
    lv_obj_set_size(list, 240,320 );
}

void create_file_menu(lv_obj_t * parent, lv_obj_t * prev_scr_ref) {
    prev_scr = prev_scr_ref;

    // Создаем кнопку "Назад"
    lv_obj_t * btn_back = lv_btn_create(parent);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(btn_back, back_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Back");

    // Создаем метку для нового меню
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "New File Selected");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

/*Change to your screen resolution*/
#if (SCREEN_ROTATION == 1) || (SCREEN_ROTATION == 3)
  static const uint16_t screenWidth  = 320;                 // rotation 1 or 3
  static const uint16_t screenHeight = 240;
#else  
  static const uint16_t screenWidth  = 240;                 // rotation 0 or 2
  static const uint16_t screenHeight = 320;
#endif

#define SIZE_SCREEN_BUFFER screenWidth * screenHeight / 4   // set screen buffer size
//#define SIZE_SCREEN_BUFFER screenWidth * 10               // smaller if build error
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ SIZE_SCREEN_BUFFER ];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

#if LV_USE_LOG != 0
// ------------------------------------------------------------------------------------------ //
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif
// ------------------------------------------------------------------------------------------ //
// Display flushing
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )  //!!

{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}
// ------------------------------------------------------------------------------------------ //
// Read the touchpad
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )  //!!modified
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        #if (SCREEN_ROTATION == 1) || (SCREEN_ROTATION == 3)
          data->point.x =  touchY;
          data->point.y =  touchX;
        #else
          data->point.x = touchX;
          data->point.y = touchY;
        #endif

        //Serial.print( "Data x " );
        //Serial.println( touchX );

        //Serial.print( "Data y " );
        //Serial.println( touchY );
    }
}
// ------------------------------------------------------------------------------------------ //
void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    tft.begin();
    tft.setRotation( SCREEN_ROTATION );

    uint16_t calData[5] = { 333, 3440, 266, 3626, 1 };

    tft.setTouch( calData );

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, SIZE_SCREEN_BUFFER );    // set Screen Buffer

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;  //!!modified
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;  //!!modified
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    lv_obj_t * base_scr = lv_scr_act();
    lv_scr_load(base_scr);
    // lv_obj_set_size(base_scr, lv_disp_get_hor_res(screenWidth), lv_disp_get_ver_res(240));  // Устанавливаем размеры списка на весь экран

    create_main_menu(base_scr);

    Serial.println( "Setup done" );
}
// ------------------------------------------------------------------------------------------ //
void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}
// ------------------------------------------------------------------------------------------ //