#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <RtcDS3231.h>
#include <Wire.h>
#include "PrayerTimes.h"
#include <DFPlayer_Mini_Mp3.h>

#define jam_pul         0
#define jam_sat         1
#define mnt_pul         2
#define mnt_sat         3
#define tgl_pul         4
#define tgl_sat         5
#define bln_pul         6
#define bln_sat         7
#define thn_rib         8
#define thn_rat         9
#define thn_pul         10
#define thn_sat         11
#define imsya_jam_pul   12
#define imsya_jam_sat   13
#define imsya_mnt_pul   14
#define imsya_mnt_sat   15
#define subuh_jam_pul   16
#define subuh_jam_sat   17
#define subuh_mnt_pul   18
#define subuh_mnt_sat   19
#define syuruq_jam_pul  20
#define syuruq_jam_sat  21
#define syuruq_mnt_pul  22
#define syuruq_mnt_sat  23
#define dhuhur_jam_pul  24
#define dhuhur_jam_sat  25
#define dhuhur_mnt_pul  26
#define dhuhur_mnt_sat  27
#define asar_jam_pul    28
#define asar_jam_sat    29
#define asar_mnt_pul    30
#define asar_mnt_sat    31
#define maghrib_jam_pul 32
#define maghrib_jam_sat 33
#define maghrib_mnt_pul 34
#define maghrib_mnt_sat 35
#define isya_jam_pul    36
#define isya_jam_sat    37
#define isya_mnt_pul    38
#define isya_mnt_sat    39
const int dat2 = 12;  // the pin with a LED
const int dat1 = 16;  // the pin with a LED
const int clk = 14;
const int lat = 0;
const int relay = 15;
const int buzz = 2;
const int manual_iqm = 13;
uint8_t sensor_value = 0;

String html ="";

#define ESP8266_TIMER0_TICKS microsecondsToClockCycles(3500)
double times[sizeof(TimeName) / sizeof(char*)];
char Essid[25];
char Epass[25];
int Lpass = 0;
int Lssid = 0;
const char *ssid = "jws_ini_ya";
const char *password = "12345678";

char r_txt[5][400];
int maks_txt[5];
int font_r_txt1[5] = {1, 1, 1, 1, 1};
int on_r_txt[5] = {1, 1, 1, 1, 1};

char r_txt_iqm[4][400];
int maks_txt_iqm[4];
int font_r_txt1_iqm[4] = {1, 1, 1, 1};
int on_r_txt_iqm[4] = {1, 1, 1, 1};

int iqomah_cnt[6] = {10, 10, 10, 10, 10};
int iqomah_jeda[6] = {10, 10, 10, 10, 10};
int manual_iqomah[6] = {0, 0, 0, 0, 0, 0};
int mode_iqomah[6] = {0, 0, 0, 0, 0};
int jml_beep_iqomah[6] = {1, 1, 1, 1, 1};
int file_adzan[6] = {1, 1, 1, 1, 1};
int if_tartil[6] = {1, 1, 1, 1, 1};
int no_file_tartil[6] = {1, 1, 1, 1, 1};
int mnt_tartil[6] = {1, 1, 1, 1, 1};

int brightness, kecepatan;


int fix_jam_imsya, fix_menit_imsya;
int fix_imsya;
int fix_jam_subuh, fix_menit_subuh;
int fix_subuh;
int fix_jam_syuruq, fix_menit_syuruq;
int fix_syuruq;
int fix_jam_dhuhur, fix_menit_dhuhur;
int fix_dhuhur;
int fix_jam_asar, fix_menit_asar;
int fix_asar;
int fix_jam_maghrib, fix_menit_maghrib;
int fix_maghrib;
int fix_jam_isya, fix_menit_isya;
int fix_isya;


byte disp_segmen[41] = {1, 2, 4, 8, 16, 32, 64, 128,
                        1, 2, 4, 8, 16, 32, 64, 128,
                        1, 2, 4, 8, 16, 32, 64, 128,
                        1, 2, 4, 8, 16, 32, 64, 128,
                        1, 2, 4, 8, 16, 32, 64, 128
                       };
byte angka3[15] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 0, 64, 124, 80};
byte huruf3[19][9] = {{0x00, 0x00, 0x77, 0x74, 0x77, 0x5E, 0x00, 0x00},
  // 0    s     e    n     i    n   0     0
  {0x00, 0x6D, 0x79, 0x37, 0x06, 0x37, 0x00, 0x00},
  // 0    s    e    l      a    s    a    0
  {0x00, 0x6D, 0x79, 0x38, 0x77, 0x6D, 0x77, 0x00},
  // 0    0   r    a     b   u     0    0
  {0x00, 0x00, 0x31, 0x77, 0x7C, 0x3E, 0x00, 0x00},
  // 0    k   a     m         i   s    0
  {0x00, 0x75, 0x77, 0x33, 0x27, 0x06, 0x6D, 0x00},
  // 0    j    u    m         a   t    0
  {0x00, 0x0E, 0x3E, 0x33, 0x27, 0x77, 0x78, 0x00},
  // 0    s    a    b    t    u    0    0
  {0x00, 0x6D, 0x77, 0x7C, 0x78, 0x3E, 0x00, 0x00},
  // 0    i    q     o    m        a    h
  {0x00, 0x06, 0x67, 0x3F, 0x33, 0x27, 0x77, 0x74},
  //0     s    h     o    l    a    t    0
  {0x00, 0x6D, 0x76, 0x3F, 0x38, 0x77, 0x78, 0x00},
  // j     a   m
  {0x0E, 0x77, 0x33, 0x27, 0x00, 0x00, 0x00, 0x00},
  // m         e   n    i    t
  {0x33, 0x27, 0x7B, 0x33, 0x06, 0x78, 0x00, 0x00},
  // 0    i    q     o    m        a    h
  {0x00, 0x06, 0x67, 0x3F, 0x33, 0x27, 0x77, 0x76},
  // j    e    d    a
  {0x0E, 0x7B, 0x5E, 0x5F, 0x00, 0x00, 0x00, 0x00},
  // i    q    m              j    u   m
  {0x06, 0x67, 0x33, 0x27, 0x00, 0x0E, 0x3E, 0x33},
  // j    e    d    a        j    u   m
  {0x0E, 0x7B, 0x5E, 0x5F, 0x00, 0x0E, 0x3E, 0x33},
  //0     0    p    l    u    s    0    0
  {0x00, 0x00, 0x73, 0x38, 0x3E, 0x6D, 0x00, 0x00},
  //0     0     m         i    n   0    0
  {0x00, 0x00, 0x33, 0x27, 0x06, 0x33, 0x00, 0x00},
  //o     n     n
  {0x3F, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00},
  //o     f     f
  {0x3F, 0x71, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00}
};
byte angka2[15];
byte huruf2[19][9];
byte angka[15] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 0, 64, 124, 80};
byte huruf[19][9] = {{0x00, 0x00, 0x5F, 0x74, 0x5F, 0x5E, 0x00, 0x00},
  // 0    s     e    n     i    n   0     0
  {0x00, 0x6D, 0x7B, 0x37, 0x06, 0x37, 0x00, 0x00},
  // 0    s    e    l      a    s    a    0
  {0x00, 0x6D, 0x7B, 0x38, 0x5F, 0x6D, 0x5F, 0x00},
  // 0    0   r    a     b   u     0    0
  {0x00, 0x00, 0x31, 0x5F, 0x7C, 0x3E, 0x00, 0x00},
  // 0    k   a     m         i   s    0
  {0x00, 0x76, 0x5F, 0x37, 0x27, 0x06, 0x6D, 0x00},
  // 0    j    u    m         a   t    0
  {0x00, 0x0E, 0x3E, 0x37, 0x27, 0x5F, 0x78, 0x00},
  // 0    s    a    b    t    u    0    0
  {0x00, 0x6D, 0x5F, 0x7C, 0x78, 0x3E, 0x00, 0x00},
  // 0    i    q     o    m        a    h
  {0x00, 0x06, 0x67, 0x3F, 0x37, 0x07, 0x5F, 0x74},
  //0     s    h     o    l    a    t    0
  {0x00, 0x6D, 0x74, 0x3F, 0x38, 0x5F, 0x78, 0x00},
  // j     a   m
  {0x0E, 0x5F, 0x37, 0x07, 0x00, 0x00, 0x00, 0x00},
  // m         e   n    i    t
  {0x37, 0x07, 0x7B, 0x37, 0x06, 0x78, 0x00, 0x00},
  // 0    i    q     o    m        a    h
  {0x00, 0x06, 0x67, 0x3F, 0x37, 0x07, 0x5F, 0x74},
  // j    e    d    a
  {0x0E, 0x7B, 0x5E, 0x5F, 0x00, 0x00, 0x00, 0x00},
  // i    q    m              j    u   m
  {0x06, 0x67, 0x37, 0x07, 0x00, 0x0E, 0x3E, 0x37},
  // j    e    d    a        j    u   m
  {0x0E, 0x7B, 0x5E, 0x5F, 0x00, 0x0E, 0x3E, 0x37},
  //0     0    p    l    u    s    0    0
  {0x00, 0x00, 0x73, 0x38, 0x3E, 0x6D, 0x00, 0x00},
  //0     0     m         i    n   0    0
  {0x00, 0x00, 0x37, 0x07, 0x06, 0x37, 0x00, 0x00},
  //o     n     n
  {0x3F, 0x37, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00},
  //o     f     f
  {0x3F, 0x71, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00}
};
int m_imsya, m_subuh, m_syuruq, m_dhuhur, m_asar, m_maghrib, m_isya;
int kur_imsya, kur_subuh, kur_syuruq, kur_dhuhur, kur_asar, kur_maghrib, kur_isya;
double lintang = -7.0667;  //Latitude
double bujur = 110.4;   //Longitude
double TimeZone = 7;
int iqm_subuh, iqm_dhuhur, iqm_asar, iqm_maghrib, iqm_isya;
int jeda_subuh, jeda_dhuhur, jeda_asar, jeda_maghrib, jeda_isya;
int iqm_jumat, jeda_jumat;
int iqomah_pos;
int jam_on = 3;
int mnt_on = 0;
int jam_off = 23;
int mnt_off = 0;
int jml_beep;
int on_off = 0;
int ada_iqomah = 0;
int iqomah_cnt_pos = 0;
int beep_iqm = 0;
int jam = 12, mnt = 34, dtk = 56, tgl = 78, bln = 90, thn = 2123;
int hari = 0, dip = 0, min_iqom = 0;
int shari = 0;
int mode_masuk_adzan = 0;
int imsya_jam = 12, imsya_mnt = 34, subuh_jam = 12, subuh_mnt = 34, syuruq_jam = 12, syuruq_mnt = 34, dhuhur_jam = 12, dhuhur_mnt = 34;
int asar_jam = 12, asar_mnt = 34, maghrib_jam = 12, maghrib_mnt = 34, isya_jam = 12, isya_mnt = 34;
int filesubuh = 1, filedhuhur = 1, fileasar = 1, filemaghrib = 1, fileisya = 1;
int bolsubuh = 1, boldhuhur = 1, bolasar = 1, bolmaghrib = 1, bolisya = 1;
int mtsubuh = 10, mtdhuhur = 10, mtasar = 10, mtmaghrib = 10, mtisya = 10;
boolean skpsubuh = false, skpdhuhur = false, skpasar = false, skpmaghrib = false, skpisya = false;

ESP8266WebServer server;
RtcDS3231<TwoWire> Rtc(Wire);

int skp = 0;
long last_pl = 0;
long jadwal() {
  int hours;
  int minutes;

  RtcDateTime now = Rtc.GetDateTime();
  tgl = now.Day();         //Tanggal
  bln = now.Month();        //Bulan
  thn = now.Year();         //Tahun
  jam = now.Hour();
  mnt = now.Minute();
  dtk = now.Second();
  hari = now.DayOfWeek();
  set_calc_method(ISNA);        //Methode perhitungan mengikuti ISNA
  set_asr_method(Shafii);       //Pendapat Imam Syafi'i
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);           //Sudut Waktu Subuh
  set_isha_angle(18);           ////Sudut Waktu Isya
  get_prayer_times(thn, bln, tgl, lintang, bujur, TimeZone, times);
  //SUBUH
  get_float_time_parts(times[0], hours, minutes);
  if (m_subuh == 0) {
    subuh_mnt = minutes + kur_subuh;
  } else {
    subuh_mnt = minutes - kur_subuh;
  }
  subuh_jam = hours;
  if (subuh_mnt > 59) {
    subuh_mnt = subuh_mnt - 60;
    subuh_jam = subuh_jam + 1;
  };
  if (subuh_mnt < 0)  {
    subuh_mnt = subuh_mnt + 60;
    subuh_jam = subuh_jam - 1;
  };
  //IMSYA
  if (subuh_mnt < 10) {
    imsya_jam = subuh_jam - 1;
    imsya_mnt = 59 - (9 - subuh_mnt);
  } else {
    imsya_jam = subuh_jam;
    imsya_mnt = (subuh_mnt - 10);
  };
  //SYURUQ
  get_float_time_parts(times[1], hours, minutes);
  if (m_syuruq == 0) {
    syuruq_mnt = minutes + kur_syuruq;
  } else {
    subuh_mnt = minutes - kur_subuh;
  }
  syuruq_jam = hours;
  if (syuruq_mnt > 59) {
    syuruq_mnt = syuruq_mnt - 60;
    syuruq_jam = syuruq_jam + 1;
  };
  if (syuruq_mnt < 0)  {
    syuruq_mnt = syuruq_mnt + 60;
    syuruq_jam = syuruq_jam - 1;
  };
  //DHUHUR
  get_float_time_parts(times[2], hours, minutes);
  if (m_dhuhur == 0) {
    dhuhur_mnt = minutes + kur_dhuhur;
  } else {
    dhuhur_mnt = minutes - kur_dhuhur;
  }
  dhuhur_jam = hours;
  if (dhuhur_mnt > 59) {
    dhuhur_mnt = dhuhur_mnt - 60;
    dhuhur_jam = dhuhur_jam + 1;
  };
  if (dhuhur_mnt < 0)  {
    dhuhur_mnt = dhuhur_mnt + 60;
    dhuhur_jam = dhuhur_jam - 1;
  };
  //ASAR
  get_float_time_parts(times[3], hours, minutes);
  if (m_asar == 0) {
    asar_mnt = minutes + kur_asar;
  } else {
    asar_mnt = minutes - kur_asar;
  }
  asar_jam = hours;
  if (asar_mnt > 59) {
    asar_mnt = asar_mnt - 60;
    asar_jam = asar_jam + 1;
  };
  if (asar_mnt < 0)  {
    asar_mnt = asar_mnt + 60;
    asar_jam = asar_jam - 1;
  };
  //MAGHRIB
  get_float_time_parts(times[4], hours, minutes);
  if (m_maghrib == 0) {
    maghrib_mnt = minutes + kur_maghrib;
  }
  else {
    maghrib_mnt = minutes - kur_maghrib;
  }
  maghrib_jam = hours;
  if (maghrib_mnt > 59) {
    maghrib_mnt = maghrib_mnt - 60;
    maghrib_jam = maghrib_jam + 1;
  };
  if (maghrib_mnt < 0)  {
    maghrib_mnt = maghrib_mnt + 60;
    maghrib_jam = maghrib_jam - 1;
  };
  //ISYA
  get_float_time_parts(times[6], hours, minutes);
  if (m_isya == 0) {
    isya_mnt = minutes + kur_isya;
  } else {
    isya_mnt = minutes - kur_isya;
  }
  isya_jam = hours;
  if (isya_mnt > 59) {
    isya_mnt = isya_mnt - 60;
    isya_jam = isya_jam + 1;
  };
  if (isya_mnt < 0)  {
    isya_mnt = isya_mnt + 60;
    isya_jam = isya_jam - 1;
  };

  if (fix_subuh == 1) {
    subuh_jam = fix_jam_subuh;
    subuh_mnt = fix_menit_subuh;
  }
  if (fix_imsya == 1) {
    imsya_jam = fix_jam_imsya;
    imsya_mnt = fix_menit_imsya;
  }
  if (fix_syuruq == 1) {
    syuruq_jam = fix_jam_syuruq;
    syuruq_mnt = fix_menit_syuruq;
  }
  if (fix_dhuhur == 1) {
    dhuhur_jam = fix_jam_dhuhur;
    dhuhur_mnt = fix_menit_dhuhur;
  }
  if (fix_asar == 1) {
    asar_jam = fix_jam_asar;
    asar_mnt = fix_menit_asar;
  }
  if (fix_maghrib == 1) {
    maghrib_jam = fix_jam_maghrib;
    maghrib_mnt = fix_menit_maghrib;
  }
  if (fix_isya == 1) {
    isya_jam = fix_jam_isya;
    isya_mnt = fix_menit_isya;
  }

  long a1 = (subuh_jam * 60) + subuh_mnt;
  long a2 = (dhuhur_jam * 60) + dhuhur_mnt;
  long a3 = (asar_jam * 60) + asar_mnt;
  long a4 = (maghrib_jam * 60) + maghrib_mnt;
  long a5 = (isya_jam * 60) + isya_mnt;

  long aa1 = ((subuh_jam * 60) + subuh_mnt) - mnt_tartil[0];
  long aa2 = ((dhuhur_jam * 60) + dhuhur_mnt) - mnt_tartil[1];
  long aa3 = ((asar_jam * 60) + asar_mnt) - mnt_tartil[2];
  long aa4 = ((maghrib_jam * 60) + maghrib_mnt) - mnt_tartil[3];
  long aa5 = ((isya_jam * 60) + isya_mnt) - mnt_tartil[4];
  long b1 = (jam * 60) + mnt;

  if (b1 == aa1) {
    if (bolsubuh == 1 && last_pl != b1) {
      last_pl = b1;
      if (if_tartil[0] == 1) {
        mp3_play(no_file_tartil[0]);
      };
    }
  }
  if (b1 == aa2) {
    if (boldhuhur == 1 && last_pl != b1) {
      last_pl = b1;
      if (hari == 5) {
        if (if_tartil[5] == 1) {
          mp3_play(no_file_tartil[5]);
        };
      } else {
        if (if_tartil[1] == 1) {
          mp3_play(no_file_tartil[1]);
        };
      }
    }
  }
  if (b1 == aa3) {
    if (bolasar == 1 && last_pl != b1) {
      last_pl = b1;
      if (if_tartil[2] == 1) {
        mp3_play(no_file_tartil[2]);
      };
    }
  }
  if (b1 == aa4) {
    if (bolmaghrib == 1 && last_pl != b1) {
      last_pl = b1;
      if (if_tartil[3] == 1) {
        mp3_play(no_file_tartil[3]);
      };
    }
  }
  if (b1 == aa5) {
    if (bolisya == 1 && last_pl != b1) {
      last_pl = b1;
      if (if_tartil[4] == 1) {
        mp3_play(no_file_tartil[4]);
      };
    }
  }

  if (b1 >= a1 && b1 < a2) {
    iqomah_pos = 1;
  };
  if (b1 >= a2 && b1 < a3) {
    iqomah_pos = 2;
  };
  if (b1 >= a3 && b1 < a4) {
    iqomah_pos = 3;
  };
  if (b1 >= a4 && b1 < a5) {
    iqomah_pos = 4;
  };
  if (b1 >= a5 || b1 < a1) {
    iqomah_pos = 5;
  };

  int lkk = 0;
  if (b1 == a1) {
    lkk = 1;
    if (skp == 0) {
      skp = 1;
      if (manual_iqomah[0] == 0) {
        ada_iqomah = 1;
        iqomah_cnt_pos = 0;
      }
      beep_iqm = jml_beep_iqomah[0];
      if (mode_iqomah[0] == 1) {
        mp3_play(file_adzan[0] + 230);
      } else {
        mp3_stop();
      }
      // beep_adzan(jml_beep, iqomah_pos);
    };
  };

  if (b1 == a2) {
    lkk = 1;
    if (skp == 0) {
      skp = 1;
      if (hari == 5) {
        if (manual_iqomah[1] == 0) {
          ada_iqomah = 1;
          iqomah_cnt_pos = 0;
        }
      } else {
        if (manual_iqomah[5] == 0) {
          ada_iqomah = 1;
          iqomah_cnt_pos = 0;
        }
      }

      beep_iqm = jml_beep_iqomah[1];
      if (mode_iqomah[1] == 1 && hari != 5) {
        mp3_play(file_adzan[1] + 230);
      } else {
        mp3_stop();
      }
      // beep_adzan(jml_beep, iqomah_pos);
    };
  };
  if (b1 == a3) {
    lkk = 1;
    if (skp == 0) {
      skp = 1;
      if (manual_iqomah[2] == 0) {
        ada_iqomah = 1;
        iqomah_cnt_pos = 0;
      }
      beep_iqm = jml_beep_iqomah[2];
      if (mode_iqomah[2] == 1) {
        mp3_play(file_adzan[2] + 230);
      } else {
        mp3_stop();
      }
      // beep_adzan(jml_beep, iqomah_pos);
    };
  };
  if (b1 == a4) {
    lkk = 1;
    if (skp == 0) {
      skp = 1;
      if (manual_iqomah[3] == 0) {
        ada_iqomah = 1;
        iqomah_cnt_pos = 0;
      }
      beep_iqm = jml_beep_iqomah[3];
      if (mode_iqomah[3] == 1) {
        mp3_play(file_adzan[3] + 230);
      } else {
        mp3_stop();
      }
      //beep_adzan(jml_beep, iqomah_pos);
    };
  };
  if (b1 == a5) {
    lkk = 1;
    if (skp == 0) {
      skp = 1;
      if (manual_iqomah[4] == 0) {
        ada_iqomah = 1;
        iqomah_cnt_pos = 0;
      }
      beep_iqm = jml_beep_iqomah[4];
      if (mode_iqomah[4] == 1) {
        mp3_play(file_adzan[4] + 230);
      } else {
        mp3_stop();
      }
      // beep_adzan(jml_beep, iqomah_pos);
    };
  };
  if (lkk == 0) {
    skp = 0;
  };

  a1 = (jam_on * 60) + mnt_on;
  a2 = (jam_off * 60) + mnt_off;
  b1 = (jam * 60) + mnt;

  if (b1 >= a1 && b1 <= a2) {
    on_off = 0;
    //digitalWrite(backlight,HIGH);
  } else {
    on_off = 1;
    clear_disp();
    //digitalWrite(backlight,LOW);
  };
}
void clear_disp() {
  jam = 200; mnt = 200; tgl = 200; bln = 200; thn = 4000;
  hari = 0; shari = 0; dip = 0; min_iqom = 0;
  imsya_jam = 200; imsya_mnt = 200;
  subuh_jam = 200; subuh_mnt = 200;
  syuruq_jam = 200; syuruq_mnt = 200;
  dhuhur_jam = 200; dhuhur_mnt = 200;
  asar_jam = 200; asar_mnt = 200;
  maghrib_jam = 200; maghrib_mnt = 200;
  isya_jam = 200; isya_mnt = 200;
  dip = 0; min_iqom = 0;
  tampil();
}

int sen(byte datar, boolean msb1, byte data1, boolean msb2) {
  for (uint8_t bit = 0; bit < 8; bit++) {
    if (msb1 == false) {
      if (datar & (1 << 7))
        digitalWrite(dat1, HIGH);
      else
        digitalWrite(dat1, LOW);
    } else {
      if (datar & 1 )
        digitalWrite(dat1, HIGH);
      else
        digitalWrite(dat1, LOW);
    }
    if (msb2 == false) {
      if (data1 & (1 << 7))
        digitalWrite(dat2, HIGH);
      else
        digitalWrite(dat2, LOW);
    } else {
      if (data1 & 1 )
        digitalWrite(dat2, HIGH);
      else
        digitalWrite(dat2, LOW);
    }
    digitalWrite(clk, HIGH);
    if (msb1 == false) {
      datar <<= 1;
    } else {
      datar >>= 1;
    };
    if (msb2 == false) {
      data1 <<= 1;
    } else {
      data1 >>= 1;
    };
    digitalWrite(clk, LOW);
  }
}

int pos_scan = 0;
void ScanDMD()
{
  byte lok[9] = {1, 2, 4, 8, 16, 32, 64, 128};
  byte lok1[9] = {128, 64, 32, 16, 8, 4, 2, 1};
  if (pos_scan > 3) {
    pos_scan = 0;
  };
  //latch 74hc595 on
  digitalWrite(lat, LOW);
  //isya
  sen(0, true, lok1[pos_scan], false);
  sen(0, true, ~disp_segmen[isya_jam_pul + pos_scan], true);
  //maghrib
  sen(0, true, lok1[pos_scan], false);
  sen(0, true, ~disp_segmen[maghrib_jam_pul + pos_scan], true);
  //ashar
  sen(lok1[pos_scan], false, lok1[pos_scan], false);
  sen(~disp_segmen[thn_rib + pos_scan], true, ~disp_segmen[asar_jam_pul + pos_scan], true);
  //dhuhur
  sen(lok1[pos_scan], false, lok1[pos_scan], false);
  sen(~disp_segmen[tgl_pul + pos_scan], true, ~disp_segmen[dhuhur_jam_pul + pos_scan], true);
  //subuh
  sen(~disp_segmen[jam_pul], false, lok1[pos_scan], false);
  sen(~disp_segmen[jam_sat], false, ~disp_segmen[subuh_jam_pul + pos_scan], true);
  //imsya
  sen(~disp_segmen[mnt_pul], false, lok1[pos_scan], false);
  sen(~disp_segmen[mnt_sat], false, ~disp_segmen[imsya_jam_pul + pos_scan], true);
  digitalWrite(lat, HIGH);
  pos_scan++;
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

void tampil() {
  //jam
  if (jam < 100) {
    disp_segmen[jam_pul] = angka3[jam / 10];
    disp_segmen[jam_sat] = angka3[jam % 10];
    if (min_iqom == 1) {
      disp_segmen[jam_pul] = angka3[11];
    };
  } else {
    disp_segmen[jam_pul] = angka3[10];
    disp_segmen[jam_sat] = angka3[10];
  }
  //menit
  if (mnt < 100) {
    disp_segmen[mnt_pul] = angka3[mnt / 10];
    disp_segmen[mnt_sat] = angka3[mnt % 10];
    if (dip == 1) {
      disp_segmen[mnt_pul] = disp_segmen[mnt_pul] | 128;
    };
  } else {
    disp_segmen[mnt_pul] = angka3[10];
    disp_segmen[mnt_sat] = angka3[10];
  }
  //tanggal
  if (tgl < 100) {
    if (shari == 0) {
      disp_segmen[tgl_pul] = angka[tgl / 10];
      disp_segmen[tgl_sat] = angka2[tgl % 10];
      disp_segmen[tgl_sat] = disp_segmen[tgl_sat] | 128;
    } else {
      disp_segmen[tgl_pul] = huruf[hari][0];
      disp_segmen[tgl_sat] = huruf2[hari][1];
    }
  } else {
    disp_segmen[tgl_pul] = angka[10];
    disp_segmen[tgl_sat] = angka2[10];
  }
  //bulan
  if (bln < 100) {
    if (shari == 0) {
      disp_segmen[bln_pul] = angka[bln / 10];
      disp_segmen[bln_sat] = angka2[bln % 10];
      disp_segmen[bln_sat] = disp_segmen[bln_sat] | 128;
    } else {
      disp_segmen[bln_pul] = huruf[hari][2];
      disp_segmen[bln_sat] = huruf2[hari][3];
    }
  } else {
    disp_segmen[bln_pul] = angka[10];
    disp_segmen[bln_sat] = angka2[10];
  }
  //tahun
  if (thn < 3000) {
    if (shari == 0) {
      disp_segmen[thn_rib] = angka[thn / 1000];
      int zx = thn % 1000;
      disp_segmen[thn_rat] = angka2[zx / 100];
      zx = zx % 100;
      disp_segmen[thn_pul] = angka[zx / 10];
      disp_segmen[thn_sat] = angka2[zx % 10];
      disp_segmen[thn_pul] = disp_segmen[thn_pul] | 128;
    } else
    {
      disp_segmen[thn_rib] = huruf[hari][4];
      disp_segmen[thn_rat] = huruf2[hari][5];
      disp_segmen[thn_pul] = huruf[hari][6];
      disp_segmen[thn_sat] = huruf2[hari][7];
    }
  } else {
    disp_segmen[thn_rib] = angka[10];
    disp_segmen[thn_rat] = angka2[10];
    disp_segmen[thn_pul] = angka[10];
    disp_segmen[thn_sat] = angka2[10];
  }
  //imysa
  if (imsya_jam < 100) {
    disp_segmen[imsya_jam_pul] = angka[imsya_jam / 10];
    disp_segmen[imsya_jam_sat] = angka2[imsya_jam % 10];
    disp_segmen[imsya_jam_sat] = disp_segmen[imsya_jam_sat] | 128;
  } else {
    disp_segmen[imsya_jam_pul] = angka[10];
    disp_segmen[imsya_jam_sat] = angka2[10];
  }
  if (imsya_mnt < 100) {
    disp_segmen[imsya_mnt_pul] = angka[imsya_mnt / 10];
    disp_segmen[imsya_mnt_sat] = angka2[imsya_mnt % 10];
  } else {
    disp_segmen[imsya_mnt_pul] = angka[10];
    disp_segmen[imsya_mnt_sat] = angka2[10];
  }
  //subuh
  if (subuh_jam < 100) {
    disp_segmen[subuh_jam_pul] = angka[subuh_jam / 10];
    disp_segmen[subuh_jam_sat] = angka2[subuh_jam % 10];
    disp_segmen[subuh_jam_sat] = disp_segmen[subuh_jam_sat]   | 128;
  } else {
    disp_segmen[subuh_jam_pul] = angka[10];
    disp_segmen[subuh_jam_sat] = angka2[10];
  }
  if (subuh_mnt < 100) {
    disp_segmen[subuh_mnt_pul] = angka[subuh_mnt / 10];
    disp_segmen[subuh_mnt_sat] = angka2[subuh_mnt % 10];
  } else {
    disp_segmen[subuh_mnt_pul] = angka[10];
    disp_segmen[subuh_mnt_sat] = angka2[10];
  }
  //syuruq
  if (syuruq_jam < 100) {
    disp_segmen[syuruq_jam_pul] = angka[syuruq_jam / 10];
    disp_segmen[syuruq_jam_sat] = angka2[syuruq_jam % 10];
    disp_segmen[syuruq_jam_sat] = disp_segmen[syuruq_jam_sat]   | 128;
  } else {
    disp_segmen[syuruq_jam_pul] = angka[10];
    disp_segmen[syuruq_jam_sat] = angka2[10];
  }
  if (syuruq_mnt < 100) {
    disp_segmen[syuruq_mnt_pul] = angka[syuruq_mnt / 10];
    disp_segmen[syuruq_mnt_sat] = angka2[syuruq_mnt % 10];
  } else {
    disp_segmen[syuruq_mnt_pul] = angka[10];
    disp_segmen[syuruq_mnt_sat] = angka2[10];
  }
  //dhuhur
  if (dhuhur_jam < 100) {
    disp_segmen[dhuhur_jam_pul] = angka[dhuhur_jam / 10];
    disp_segmen[dhuhur_jam_sat] = angka2[dhuhur_jam % 10];
    disp_segmen[dhuhur_jam_sat]  = disp_segmen[dhuhur_jam_sat]  | 128;
  } else {
    disp_segmen[dhuhur_jam_pul] = angka[10];
    disp_segmen[dhuhur_jam_sat] = angka2[10];
  }
  if (dhuhur_mnt < 100) {
    disp_segmen[dhuhur_mnt_pul] = angka[dhuhur_mnt / 10];
    disp_segmen[dhuhur_mnt_sat] = angka2[dhuhur_mnt % 10];
  } else {
    disp_segmen[dhuhur_mnt_pul] = angka[10];
    disp_segmen[dhuhur_mnt_sat] = angka2[10];
  }
  //asar
  if (asar_jam < 100) {
    disp_segmen[asar_jam_pul] = angka[asar_jam / 10];
    disp_segmen[asar_jam_sat] = angka2[asar_jam % 10];
    disp_segmen[asar_jam_sat]    = disp_segmen[asar_jam_sat]    | 128;
  } else {
    disp_segmen[asar_jam_pul] = angka[10];
    disp_segmen[asar_jam_sat] = angka2[10];
  }
  if (asar_mnt < 100) {
    disp_segmen[asar_mnt_pul] = angka[asar_mnt / 10];
    disp_segmen[asar_mnt_sat] = angka2[asar_mnt % 10];
  } else {
    disp_segmen[asar_mnt_pul] = angka[10];
    disp_segmen[asar_mnt_sat] = angka2[10];
  }
  //maghrib
  if (maghrib_jam < 100) {
    disp_segmen[maghrib_jam_pul] = angka[maghrib_jam / 10];
    disp_segmen[maghrib_jam_sat] = angka2[maghrib_jam % 10];
    disp_segmen[maghrib_jam_sat] = disp_segmen[maghrib_jam_sat] | 128;
  } else {
    disp_segmen[maghrib_jam_pul] = angka[10];
    disp_segmen[maghrib_jam_sat] = angka2[10];
  }
  if (maghrib_mnt < 100) {
    disp_segmen[maghrib_mnt_pul] = angka[maghrib_mnt / 10];
    disp_segmen[maghrib_mnt_sat] = angka2[maghrib_mnt % 10];
  } else {
    disp_segmen[maghrib_mnt_pul] = angka[10];
    disp_segmen[maghrib_mnt_sat] = angka2[10];
  }
  //isya
  if (isya_jam < 100) {
    disp_segmen[isya_jam_pul] = angka[isya_jam / 10];
    disp_segmen[isya_jam_sat] = angka2[isya_jam % 10];
    disp_segmen[isya_jam_sat]    = disp_segmen[isya_jam_sat]    | 128;
  } else {
    disp_segmen[isya_jam_pul] = angka[10];
    disp_segmen[isya_jam_sat] = angka2[10];
  }
  if (isya_mnt < 100) {
    disp_segmen[isya_mnt_pul] = angka[isya_mnt / 10];
    disp_segmen[isya_mnt_sat] = angka2[isya_mnt % 10];
  } else {
    disp_segmen[isya_mnt_pul] = angka[10];
    disp_segmen[isya_mnt_sat] = angka2[10];
  }
}
int konv_to_int(String txt1) {
  char temp[5];
  int y = txt1.length() + 1;
  for (int a = 0; a < y; a++) {
    temp[a] = txt1[a];
  };
  int nnn = atoi(temp);
  return nnn;
}
void disp_option() {
  timer0_detachInterrupt();
  server.sendHeader("Acces-Control-Allow-Origin", "*");
  server.send(204, "");
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}
int ambil_txt_prop() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/txt_prop.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int ambil_wifi() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/wireless.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int get_wf() {
  File configFile = SPIFFS.open("/wireless.json", "r");
  if (!configFile) {
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    return false;
  }
  configFile.close();
  String txt111 = json["sid"];
  int y = txt111.length();
  Lssid = y;
  for (int a = 0; a < y; a++) {
    Essid[a] = txt111[a];
  };
  String txt222 = json["pas"];
  y = txt222.length();
  Lpass = y;
  for (int a = 0; a < y; a++) {
    Epass[a] = txt222[a];
  };
}
int set_wf() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/wireless.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();
  json["sid"] = server.arg("ssid");
  json["pas"] = server.arg("pass");
  configFile = SPIFFS.open("/wireless.json", "w");
  json.printTo(configFile);
  server.send(200, "text/plain", "ok");
  configFile.close();
  get_wf();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int set_wf_fix() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/wireless.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();
  json["pas"] = "12345678";
  configFile = SPIFFS.open("/wireless.json", "w");
  json.printTo(configFile);
  server.send(200, "text/plain", "ok");
  configFile.close();
  get_wf();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int ambil_board() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/prop.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}
int get_bd() {
  File configFile = SPIFFS.open("/prop.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();
  jam_on = json["jam_on"];
  mnt_on = json["menit_on"];
  jam_off = json["jam_off"];
  mnt_off = json["menit_off"];
}

int get_txt_prop() {
  File configFile = SPIFFS.open("/txt_prop.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();
  brightness = json["brightess"];
  kecepatan = json["kecepatan"];
}

int set_txt_prop() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/txt_prop.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();
  json["brightess"] = konv_to_int(server.arg("brightness"));
  json["kecepatan"] = konv_to_int(server.arg("kecepatan"));

  configFile = SPIFFS.open("/txt_prop.json", "w");
  json.printTo(configFile);
  configFile.close();
  server.send(200, "text/plain", "ok");
  get_txt_prop();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int set_bd() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/prop.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();
  json["jam_on"] = konv_to_int(server.arg("jam_on"));
  json["menit_on"] = konv_to_int(server.arg("menit_on"));
  json["jam_off"] = konv_to_int(server.arg("jam_off"));
  json["menit_off"] = konv_to_int(server.arg("menit_off"));
  configFile = SPIFFS.open("/prop.json", "w");
  json.printTo(configFile);
  configFile.close();
  server.send(200, "text/plain", "ok");
  get_bd();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}
int set_kalender() {
  timer0_detachInterrupt();
  String temm = server.arg("tanggal");
  int len = temm.length();
  char ang[len];
  for (int i = 0; i < len; i++) {
    ang[i] = temm[i];
  };
  int tanggal = atoi(ang);

  String temm1 = server.arg("bulan");
  len = temm1.length();
  char ang1[len];
  for (int i = 0; i < len; i++) {
    ang1[i] = temm1[i];
  };
  int bulan = atoi(ang1);

  String temm2 = server.arg("tahun");
  len = temm2.length();
  char ang2[len];
  for (int i = 0; i < len; i++) {
    ang2[i] = temm2[i];
  };
  int tahun = atoi(ang2);

  String temm3 = server.arg("jam");
  len = temm3.length();
  char ang3[len];
  for (int i = 0; i < len; i++) {
    ang3[i] = temm3[i];
  };
  int jam = atoi(ang3);

  String temm4 = server.arg("menit");
  len = temm4.length();
  char ang4[len];
  for (int i = 0; i < len; i++) {
    ang4[i] = temm4[i];
  };
  int menit = atoi(ang4);

  String temm5 = server.arg("detik");
  len = temm5.length();
  char ang5[len];
  for (int i = 0; i < len; i++) {
    ang5[i] = temm5[i];
  };
  int detik = atoi(ang5);

  RtcDateTime currentTime = RtcDateTime(tahun, bulan, tanggal, jam, menit, detik); //define date and time object
  Rtc.SetDateTime(currentTime);

  server.send(200, "text/plain", "ok");
  jadwal();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int get_txt() {
  File configFile = SPIFFS.open("/txt_umum.json", "r");
  if (!configFile) {
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.parseArray(buf.get());
  //JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    return false;
  }
  configFile.close();
  for (int a = 0; a < 5; a++) {
    for (int b = 0; b < 400; b++) {
      r_txt[a][b] = 0;
    };
  };
  for (int a = 0; a < 5; a++) {
    font_r_txt1[a] = konv_to_int(json[a]["font"]);
  }
  for (int a = 0; a < 5; a++) {
    on_r_txt[a] = konv_to_int(json[a]["active"]);
  }
  for (int a = 0; a < 5; a++) {
    String tx2 = json[a]["text"];
    int y = tx2.length();
    maks_txt[a] = y;
    for (int b = 0; b < y; b++) {
      r_txt[a][b] = tx2[b];
    };
  }
}

int get_txt_iqm() {
  File configFile = SPIFFS.open("/txt_iqomah.json", "r");
  if (!configFile) {
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.parseArray(buf.get());
  //JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    return false;
  }
  configFile.close();
  for (int a = 0; a < 4; a++) {
    for (int b = 0; b < 400; b++) {
      r_txt_iqm[a][b] = 0;
    };
  };
  for (int a = 0; a < 4; a++) {
    font_r_txt1_iqm[a] = konv_to_int(json[a]["font"]);
  }
  for (int a = 0; a < 4; a++) {
    on_r_txt_iqm[a] = konv_to_int(json[a]["active"]);
  }
  for (int a = 0; a < 4; a++) {
    String tx2 = json[a]["text"];
    int y = tx2.length();
    maks_txt_iqm[a] = y;
    for (int b = 0; b < y; b++) {
      r_txt_iqm[a][b] = tx2[b];
    };
  }
}
int take_iqomah() {
  File configFile = SPIFFS.open("/iqomah.json", "r");
  if (!configFile) {
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.parseArray(buf.get());
  //JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    return false;
  }
  configFile.close();

  for (int a = 0; a < 6; a++) {
    iqomah_cnt[a] = konv_to_int(json[a]["iqomah"]);
    iqomah_jeda[a] = konv_to_int(json[a]["jeda"]);
    mode_iqomah[a] = konv_to_int(json[a]["mode_masuk"]);
    jml_beep_iqomah[a] = konv_to_int(json[a]["beep"]);
    file_adzan[a] = konv_to_int(json[a]["adzan"]);
    if_tartil[a] = konv_to_int(json[a]["tartil"]);
    no_file_tartil[a] = konv_to_int(json[a]["no_file"]);
    mnt_tartil[a] = konv_to_int(json[a]["mnt_tartil"]);
  }

}
int txt_pesan() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/txt_umum.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int txt_pesan_iqm() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/txt_iqomah.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}
int amb_iqomah() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/iqomah.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int updt_txt_pesan()
{
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/txt_umum.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.parseArray(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  configFile.close();

  String txt15 = server.arg("no");
  int nom = konv_to_int(txt15);

  json[nom]["active"] = server.arg("active");
  json[nom]["text"] = server.arg("text");
  json[nom]["font"] = server.arg("font");

  configFile = SPIFFS.open("/txt_umum.json", "w");
  json.printTo(configFile);
  server.send(200, "text/plain", "ok");
  get_txt();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int updt_txt_pesan_iqm()
{
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/txt_iqomah.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.parseArray(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  configFile.close();

  String txt15 = server.arg("no");
  int nom = konv_to_int(txt15);

  json[nom]["active"] = server.arg("active");
  json[nom]["text"] = server.arg("text");
  json[nom]["font"] = server.arg("font");

  configFile = SPIFFS.open("/txt_iqomah.json", "w");
  json.printTo(configFile);
  server.send(200, "text/plain", "ok");
  get_txt_iqm();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}
int updt_iqqm()
{
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/iqomah.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.parseArray(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  configFile.close();
  String txt15 = server.arg("no");
  int nom = konv_to_int(txt15);
  json[nom]["iqomah"] = server.arg("iqomah");
  json[nom]["jeda"] = server.arg("jeda");
  json[nom]["mode_masuk"] = server.arg("mode_masuk");
  json[nom]["beep"] = server.arg("beep");
  json[nom]["adzan"] = server.arg("adzan");
  json[nom]["tartil"] = server.arg("tartil");
  json[nom]["no_file"] = server.arg("no_file");
  json[nom]["mnt_tartil"] = server.arg("mnt_tartil");
  configFile = SPIFFS.open("/iqomah.json", "w");
  json.printTo(configFile);
  server.send(200, "text/plain", "ok");
  take_iqomah();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int ambil_manual() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/manual.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int ambil_jadwalsholat() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/jadwalsholat.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int get_manual() {
  File configFile = SPIFFS.open("/manual.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();
  manual_iqomah[0] = json["manual_subuh"];
  manual_iqomah[1] = json["manual_dhuhur"];
  manual_iqomah[2] = json["manual_asar"];
  manual_iqomah[3] = json["manual_maghrib"];
  manual_iqomah[4] = json["manual_isya"];
  manual_iqomah[5] = json["manual_jumat"];
}

int set_manual() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/manual.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();

  json["manual_subuh"] = konv_to_int(server.arg("manual_subuh"));
  json["manual_dhuhur"] = konv_to_int(server.arg("manual_dhuhur"));
  json["manual_asar"] = konv_to_int(server.arg("manual_asar"));
  json["manual_maghrib"] = konv_to_int(server.arg("manual_maghrib"));
  json["manual_isya"] = konv_to_int(server.arg("manual_isya"));
  json["manual_jumat"] = konv_to_int(server.arg("manual_jumat"));

  configFile = SPIFFS.open("/manual.json", "w");
  json.printTo(configFile);
  configFile.close();
  server.send(200, "text/plain", "ok");
  get_manual();
  jadwal();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}



int get_jw() {
  File configFile = SPIFFS.open("/jadwalsholat.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();
  m_subuh = json["min_subuh"];
  m_syuruq = json["min_syuruq"];
  m_dhuhur = json["min_dhuhur"];
  m_asar = json["min_asar"];
  m_maghrib = json["min_maghrib"];
  m_isya = json["min_isya"];

  kur_subuh = json["subuh"];
  kur_syuruq = json["syuruq"];
  kur_dhuhur = json["dhuhur"];
  kur_asar = json["asar"];
  kur_maghrib = json["maghrib"];
  kur_isya = json["isya"];

  lintang = json["Latitude"];
  bujur = json["Longitude"];
  TimeZone = json["TimeZone"];
}


int set_jws() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/jadwalsholat.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();

  json["subuh"] = konv_to_int(server.arg("subuh"));
  json["syuruq"] = konv_to_int(server.arg("syuruq"));
  json["dhuhur"] = konv_to_int(server.arg("dhuhur"));
  json["asar"] = konv_to_int(server.arg("asar"));
  json["maghrib"] = konv_to_int(server.arg("maghrib"));
  json["isya"] = konv_to_int(server.arg("isya"));

  String mer = server.arg("latitude");
  json["Latitude"] = mer.toFloat();
  String mer1 = server.arg("longitude");
  json["Longitude"] = mer1.toFloat();
  String mer2 = server.arg("timezone");
  json["TimeZone"] = mer2.toFloat();

  json["min_subuh"] = konv_to_int(server.arg("min_subuh"));
  json["min_syuruq"] = konv_to_int(server.arg("min_syuruq"));
  json["min_dhuhur"] = konv_to_int(server.arg("min_dhuhur"));
  json["min_asar"] = konv_to_int(server.arg("min_asar"));
  json["min_maghrib"] = konv_to_int(server.arg("min_maghrib"));
  json["min_isya"] = konv_to_int(server.arg("min_isya"));
  configFile = SPIFFS.open("/jadwalsholat.json", "w");
  json.printTo(configFile);
  configFile.close();
  server.send(200, "text/plain", "ok");
  get_jw();
  jadwal();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int playTartil() {
  int nom = konv_to_int(server.arg("no"));
  mp3_play (nom);
  server.send(200, "text/plain", "ok");
}

int playAdzan() {
  int nom = konv_to_int(server.arg("no"));
  mp3_play (nom + 230);
  server.send(200, "text/plain", "ok");
}

int fixnya() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/fixx.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = server.streamFile(configFile, "text/html");
  configFile.close();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}

int get_fixnya() {
  File configFile = SPIFFS.open("/fixx.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();

  fix_imsya = json["fix_imsya"];
  fix_subuh = json["fix_subuh"];
  fix_syuruq = json["fix_syuruq"];
  fix_dhuhur = json["fix_dhuhur"];
  fix_asar = json["fix_asar"];
  fix_maghrib = json["fix_maghrib"];
  fix_isya = json["fix_isya"];

  fix_jam_imsya = json["fix_jam_imsya"];
  fix_jam_subuh = json["fix_jam_subuh"];
  fix_jam_syuruq = json["fix_jam_syuruq"];
  fix_jam_dhuhur = json["fix_jam_dhuhur"];
  fix_jam_asar = json["fix_jam_asar"];
  fix_jam_maghrib = json["fix_jam_maghrib"];
  fix_jam_isya = json["fix_jam_isya"];

  fix_menit_imsya = json["fix_menit_imsya"];
  fix_menit_subuh = json["fix_menit_subuh"];
  fix_menit_syuruq = json["fix_menit_syuruq"];
  fix_menit_dhuhur = json["fix_menit_dhuhur"];
  fix_menit_asar = json["fix_menit_asar"];
  fix_menit_maghrib = json["fix_menit_maghrib"];
  fix_menit_isya = json["fix_menit_isya"];

}

int updt_fixnya() {
  timer0_detachInterrupt();
  File configFile = SPIFFS.open("/fixx.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "file tidak ada");
    return false;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    server.send(404, "text/plain", "gagal baca file json");
    return false;
  }
  configFile.close();

  json["fix_imsya"] = konv_to_int(server.arg("fix_imsya"));
  json["fix_subuh"] = konv_to_int(server.arg("fix_subuh"));
  json["fix_syuruq"] = konv_to_int(server.arg("fix_syuruq"));
  json["fix_dhuhur"] = konv_to_int(server.arg("fix_dhuhur"));
  json["fix_asar"] = konv_to_int(server.arg("fix_asar"));
  json["fix_maghrib"] = konv_to_int(server.arg("fix_maghrib"));
  json["fix_isya"] = konv_to_int(server.arg("fix_isya"));

  json["fix_jam_imsya"] = konv_to_int(server.arg("fix_jam_imsya"));
  json["fix_jam_subuh"] = konv_to_int(server.arg("fix_jam_subuh"));
  json["fix_jam_syuruq"] = konv_to_int(server.arg("fix_jam_syuruq"));
  json["fix_jam_dhuhur"] = konv_to_int(server.arg("fix_jam_dhuhur"));
  json["fix_jam_asar"] = konv_to_int(server.arg("fix_jam_asar"));
  json["fix_jam_maghrib"] = konv_to_int(server.arg("fix_jam_maghrib"));
  json["fix_jam_isya"] = konv_to_int(server.arg("fix_jam_isya"));

  json["fix_menit_imsya"] = konv_to_int(server.arg("fix_menit_imsya"));
  json["fix_menit_subuh"] = konv_to_int(server.arg("fix_menit_subuh"));
  json["fix_menit_syuruq"] = konv_to_int(server.arg("fix_menit_syuruq"));
  json["fix_menit_dhuhur"] = konv_to_int(server.arg("fix_menit_dhuhur"));
  json["fix_menit_asar"] = konv_to_int(server.arg("fix_menit_asar"));
  json["fix_menit_maghrib"] = konv_to_int(server.arg("fix_menit_maghrib"));
  json["fix_menit_isya"] = konv_to_int(server.arg("fix_menit_isya"));

  configFile = SPIFFS.open("/fixx.json", "w");
  json.printTo(configFile);
  configFile.close();
  server.send(200, "text/plain", "ok");
  get_fixnya();
  jadwal();
  timer0_attachInterrupt(ScanDMD);
  timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
}


int ada_iqmm1() {
  if (ada_iqomah == 0) {
    jadwal();
    ada_iqomah = 1;
    iqomah_cnt_pos = 0;

  } else {
    ada_iqomah = 0;
    iqomah_cnt_pos = 0;

  };
  //server.send(200, "text/plain", "ok");
}

void setup() {
  // put your setup code here, to run once:
  for (int a = 0; a < 15; a++) {
    angka[a] = 0;
    angka2[a] = 0;
    if (angka3[a] & 1)  {
      angka[a] = angka[a] | 4;
      angka2[a] = angka2[a]  | 2;
    };          //
    if (angka3[a] & 2)  {
      angka[a] = angka[a] | 8;
      angka2[a] = angka2[a]  | 1;
    };             //
    if (angka3[a] & 4)  {
      angka[a] = angka[a] | 16;
      angka2[a] = angka2[a]  | 64;
    };  ///
    if (angka3[a] & 8)  {
      angka[a] = angka[a] | 32;
      angka2[a] = angka2[a]  | 32;
    };           ///

    if (angka3[a] & 16) {
      angka[a] = angka[a] | 64;
      angka2[a] = angka2[a]  | 16;
    };                  //
    if (angka3[a] & 32) {
      angka[a] = angka[a] | 2;
      angka2[a] = angka2[a]  | 4;
    };       //
    if (angka3[a] & 64) {
      angka[a] = angka[a] | 1;
      angka2[a] = angka2[a]  | 8;
    };      //
    if (angka3[a] & 128) {
      angka[a] = angka[a] | 128;
      angka2[a] = angka2[a]  | 128;
    };
  }
  for (int a = 0; a < 19; a++) {
    for (int b = 0; b < 9; b++) {
      huruf[a][b] = 0; huruf2[a][b] = 0;
      if (huruf3[a][b] & 1)  {
        huruf[a][b] = huruf[a][b] | 4;
        huruf2[a][b] = huruf2[a][b]  | 2;
      };   //A
      if (huruf3[a][b] & 2)  {
        huruf[a][b] = huruf[a][b] | 8;
        huruf2[a][b] = huruf2[a][b]  | 1;
      };   //B
      if (huruf3[a][b] & 4)  {
        huruf[a][b] = huruf[a][b] | 16;
        huruf2[a][b] = huruf2[a][b]  | 64;
      };   //C
      if (huruf3[a][b] & 8)  {
        huruf[a][b] = huruf[a][b] | 32;
        huruf2[a][b] = huruf2[a][b]  | 32;
      };  //D
      if (huruf3[a][b] & 16) {
        huruf[a][b] = huruf[a][b] | 64;
        huruf2[a][b] = huruf2[a][b]  | 16;
      };   //E
      if (huruf3[a][b] & 32) {
        huruf[a][b] = huruf[a][b] | 2;
        huruf2[a][b] = huruf2[a][b]  | 4;
      };   //F
      if (huruf3[a][b] & 64) {
        huruf[a][b] = huruf[a][b] | 1;
        huruf2[a][b] = huruf2[a][b]  | 8;
      };  //G
      if (huruf3[a][b] & 128) {
        huruf[a][b] = huruf[a][b] | 128;
        huruf2[a][b] = huruf2[a][b]  | 128;
      }; //H
    }
  }
  pinMode(dat1, OUTPUT);
  pinMode(dat2, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(lat, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(manual_iqm, INPUT_PULLUP);
  digitalWrite(buzz, LOW);
  digitalWrite(relay, LOW);

  for (int az = 0; az < 2; az++) {
    delay(1000);
  }
  if (!SPIFFS.begin()) {
  }
  get_wf();
  get_bd();
  get_txt();
  get_txt_iqm();
  get_jw();
  take_iqomah();
  get_txt_prop();
  get_fixnya();
  get_manual();

  String fssid;
  for (int n = 0; n < Lssid; n++) {
    fssid += Essid[n];
  };
  String fpass;
  for (int n = 0; n < Lpass; n++) {
    fpass += Epass[n];
  };
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  if (Lpass == 8) {
    WiFi.softAP(fssid.c_str(), fpass.c_str());
  } else
  {
    WiFi.softAP(ssid, password);
  };
  server.on("/", []() {
    html ="<!DOCTYPE html>";
    html +="<style>";
    html +="html {height: 100%; background: radial-gradient(rgb(50,50,50), rgb(40,40,40), rgb(20,20,20));}";
    html +="h1 {display: block; margin:auto; font-family: Arial ; font-weight: bold; font-size: 2000%;color: white; text-align: center; position: relative;margin-top:0px;}";
    html +="p1 {display: block; margin:auto; font-family: Arial ; font-weight: bold; font-size: 2500%;color: white; text-align: center; position: relative; margin-top:30px;}";
    html +="button {display: block; margin:auto;height: 400px; width: 400px; font-family: Arial ;font-size: 800%; font-weight: bold; border-radius: 50%; outline-style: none; position:relative; margin-top:150 px; }";
    html +="#dataVals { max-width: 400px; display: block; margin-top: 30px; }";
    html +="</style>";
    html +="</head>";
    html +="<body>";
    html +="<h1>Counter_value</h1>";
    html +="<p1 id=\"sensor_value\"></p1>";
    html +="<button onclick=zero()> Zero </button>";
    html +="<script>";
    html +="var changed = false;";
    html +="function loadDoc() {";
    html +="    var xhttp = new XMLHttpRequest();";
    html +="    xhttp.onreadystatechange = function() {";
    html +="        if (this.readyState == 4 && this.status == 200) {";
    html +="        var obj = JSON.parse(this.responseText);";
    html +="        document.getElementById(\"sensor_value\").innerHTML = obj.data[0].dataValue;";
    html +="        }";
    html +="    };";
    html +="    xhttp.open(\"GET\", \"/data\", true);";
    html +="    xhttp.send();";
    html +="}";
    html +="function zero(){";
    html +=" zero = document.getElementById(\"sensor_value\").innerHTML;";
    html +="}";
    html +="}";
    html +="var timedEvent = setInterval(function(){ loadDoc(); }, 200);";
    html +="</script>";
    html +="</body> </html>";
    server.send(200, "text/html", html);
  });
  server.on("/take_wf", ambil_wifi); server.on("/send_wf", HTTP_OPTIONS, disp_option);
  server.on("/take_txt_prop", ambil_txt_prop); server.on("/send_txt_prop", HTTP_OPTIONS, disp_option); server.on("/send_txt_prop", HTTP_POST, set_txt_prop);
  server.on("/send_wf", HTTP_POST, set_wf); server.on("/amb_iqqm", amb_iqomah);
  server.on("/take_bd", ambil_board); server.on("/send_bord", HTTP_OPTIONS, disp_option); server.on("/amb_iqqm", HTTP_OPTIONS, disp_option); server.on("/amb_iqqm", HTTP_POST, amb_iqomah);
  server.on("/send_bord", HTTP_POST, set_bd); server.on("/send_kl", HTTP_OPTIONS, disp_option);
  server.on("/send_kl", HTTP_POST, set_kalender); server.on("/txt_pesan", txt_pesan);
  server.on("/txt_pesan", HTTP_OPTIONS, disp_option); server.on("/txt_pesan", HTTP_POST, txt_pesan);
  server.on("/updt_txt_pesan", HTTP_OPTIONS, disp_option); server.on("/updt_txt_pesan", HTTP_POST, updt_txt_pesan);
  server.on("/txt_pesan_iqm", txt_pesan_iqm); server.on("/take_jw", ambil_jadwalsholat); server.on("/send_jw", HTTP_OPTIONS, disp_option);
  server.on("/send_jw", HTTP_POST, set_jws); server.on("/txt_pesan_iqm", HTTP_OPTIONS, disp_option); server.on("/txt_pesan_iqm", HTTP_POST, txt_pesan_iqm);
  server.on("/updt_txt_pesan_iqm", HTTP_OPTIONS, disp_option); server.on("/updt_txt_pesan_iqm", HTTP_POST, updt_txt_pesan_iqm);
  server.on("/updt_iqqm", HTTP_OPTIONS, disp_option); server.on("/updt_iqqm", HTTP_POST, updt_iqqm);
  server.on("/fixnya", fixnya);
  server.on("/updt_fixnya", HTTP_OPTIONS, disp_option);
  server.on("/updt_fixnya", HTTP_POST, updt_fixnya);
  server.on("/manman", ambil_manual);
  server.on("/updt_manman", HTTP_OPTIONS, disp_option);
  server.on("/updt_manman", HTTP_POST, set_manual);

  server.on("/playTartil", HTTP_OPTIONS, disp_option);
  server.on("/playTartil", HTTP_POST, playTartil);
  server.on("/playAdzan", HTTP_OPTIONS, disp_option);
  server.on("/playAdzan", HTTP_POST, playAdzan);
  server.begin(); Rtc.Begin();
  Serial.begin(9600);
  mp3_set_serial (Serial);
  mp3_set_volume (20);

  timer0_isr_init(); timer0_attachInterrupt(ScanDMD); timer0_write(ESP.getCycleCount() + ESP8266_TIMER0_TICKS);
  jadwal(); tampil();
  tampil();

}

boolean tb_iqm = false;
long mil_tb_iqm = millis();
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  handle_serial();
  if (!digitalRead(manual_iqm)) {
    if (tb_iqm == false) {
      tb_iqm = true;
      mil_tb_iqm = millis();
      ada_iqmm1();
      //Serial.println("tmb");
    } else {
      if ((millis() - mil_tb_iqm) > 800) {
        tb_iqm = false;
      }
    }
  }
  if (ada_iqomah == 1) {
    handle_iqm();
  } else {
    digitalWrite(buzz, LOW);
    if (on_off == 0) {
      handle_rutin();
    } else {
      handle_cek_jadwal();
    }
  }
}



long millis_iqm = millis();
int ciqm = 0;
int jiqm = 0;
int mnnt = 0;
int jumat_pos = 0;
int pos_jam, pos_mnt;
int detek;
boolean pingpong = false;
int saatnya = 0;
int beepi = 0;
void handle_iqm() {
  if (iqomah_cnt_pos == 0) {
    switch (iqomah_pos) {
      case 1:
        ciqm = iqomah_cnt[0];
        jiqm = iqomah_jeda[0];
        jumat_pos = 0;
        pos_jam = subuh_jam;         pos_mnt = subuh_mnt;
        break;
      case 2:
        if (hari == 5) {
          ciqm = iqomah_cnt[5];
          jiqm = iqomah_jeda[5];
          jumat_pos = 1;
        } else {
          ciqm = iqomah_cnt[1];
          jiqm = iqomah_jeda[1];
          jumat_pos = 0;
        }
        pos_jam = dhuhur_jam;         pos_mnt = dhuhur_mnt;
        break;
      case 3:
        ciqm = iqomah_cnt[2];
        jiqm = iqomah_jeda[2];
        jumat_pos = 0;
        pos_jam = asar_jam;         pos_mnt = asar_mnt;
        break;
      case 4:
        ciqm = iqomah_cnt[3];
        jiqm = iqomah_jeda[3];
        jumat_pos = 0;
        pos_jam = maghrib_jam;         pos_mnt = maghrib_mnt;
        break;
      case 5:
        ciqm = iqomah_cnt[4];
        jiqm = iqomah_jeda[4];
        jumat_pos = 0;
        pos_jam = isya_jam;         pos_mnt = isya_mnt;
        break;
    }
    clear_disp();
    hari = 7; // jadikan tanggal tulisan iqomah
    mnnt = 0;
    shari = 1;
    tampil();
    saatnya = 5;
    beepi = beep_iqm;
    detek = ciqm - 1;
    iqomah_cnt_pos = 1;
  }

  if (iqomah_cnt_pos == 1) {

    if ((millis() - millis_iqm) < 500) {
      return;
    } else {
      millis_iqm = millis();
    }

    if (pingpong == false) {
      pingpong = true;
      RtcDateTime now = Rtc.GetDateTime();
      tgl = now.Day(); bln = now.Month(); thn = now.Year();
      dip = 1; min_iqom = 0;
      switch (iqomah_pos) {
        case 1:
          subuh_jam = pos_jam;    subuh_mnt = pos_mnt;
          break;
        case 2:
          dhuhur_jam = pos_jam;   dhuhur_mnt = pos_mnt;
          break;
        case 3:
          asar_jam = pos_jam;     asar_mnt = pos_mnt;
          break;
        case 4:
          maghrib_jam = pos_jam;  maghrib_mnt = pos_mnt;
          break;
        case 5:
          isya_jam = pos_jam;     isya_mnt = pos_mnt;
          break;
      }
      if (mode_iqomah[iqomah_pos - 1] == 0) {
        if (detek == ciqm && beepi > 0) {
          digitalWrite(buzz, HIGH);
          if (beepi > 0) {
            beepi--;
          };
        }
      };
      if (ciqm == 0 && mnnt < beep_iqm + 2) {
        digitalWrite(buzz, HIGH);
      }
      //beepi = beep_iqm;
    } else {
      if (ciqm == 0 && mnnt < 3) {
      } else {
        digitalWrite(buzz, LOW);
      }
      pingpong = false;
      tgl = 200; bln = 200; thn = 4000;
      dip = 0; min_iqom = 0;
      switch (iqomah_pos) {
        case 1:
          subuh_jam = 200;    subuh_mnt = 200;
          break;
        case 2:
          dhuhur_jam = 200;   dhuhur_mnt = 200;
          break;
        case 3:
          asar_jam = 200;     asar_mnt = 200;
          break;
        case 4:
          maghrib_jam = 200;  maghrib_mnt = 200;
          break;
        case 5:
          isya_jam = 200;     isya_mnt = 200;
          break;
      }
      if (mnnt > 0) {
        mnnt--;
      } else {
        if (ciqm > 0) {
          ciqm--;
          mnnt = 59;

        }
      };
    }
    jam = ciqm; mnt = mnnt;
    if (ciqm == 0 && mnnt == 0) {
      digitalWrite(buzz, LOW);
      iqomah_cnt_pos = 2;
      clear_disp();
      hari = 8; // jadikan tanggal tulisan iqomah
      shari = 1;
    }
    //beepi = beep_iqm;
    tampil();
  }

  if (iqomah_cnt_pos == 2) {
    if ((millis() - millis_iqm) < 500) {
      return;
    } else {
      millis_iqm = millis();
    }

    if (pingpong == false) {
      pingpong = true;
      RtcDateTime now = Rtc.GetDateTime();
      tgl = now.Day(); bln = now.Month(); thn = now.Year();
    } else {
      pingpong = false;
      tgl = 200; bln = 200; thn = 4000;
      if (mnnt > 0) {
        mnnt--;
      } else {
        if (jiqm > 0) {
          jiqm--;
          mnnt = 59;
        }
      };
    }
    if (jiqm == 0 && mnnt == 0) {
      ada_iqomah = 0; dip = 0;
      iqomah_cnt_pos = 3;
    }
    tampil();
  }

}
//iqomah_pos
//iqomah_cnt_pos
//beep_iqm
//int iqomah_cnt[5] = {10, 10, 10, 10, 10};
//int iqomah_jeda[5] = {10, 10, 10, 10, 10};
//int mode_iqomah[5] = {0, 0, 0, 0, 0};
//int jml_beep_iqomah[5] = {1, 1, 1, 1, 1};
//int file_adzan[5] = {1, 1, 1, 1, 1};
//int if_tartil[5] = {1, 1, 1, 1, 1};
//int no_file_tartil[5] = {1, 1, 1, 1, 1};
//int mnt_tartil[5] = {1, 1, 1, 1, 1};

//char r_txt[5][400];
//int maks_txt[5];
//int font_r_txt1[5] = {1, 1, 1, 1, 1};
//int on_r_txt[5] = {1, 1, 1, 1, 1};

//char r_txt_iqm[4][400];
//int maks_txt_iqm[4];
//int font_r_txt1_iqm[4] = {1, 1, 1, 1};
//int on_r_txt_iqm[4] = {1, 1, 1, 1};

int tex_pos = 0;
void handle_serial() {
  if (Serial.available() > 0)
  {
    String inData = Serial.readStringUntil('\n');
    Serial.flush();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(inData);
    if (!json.success()) {
      return;
    }
    int minta = json["minta"];
    if (minta == 1) {
      if (ada_iqomah == 1) {
        if (iqomah_cnt_pos > 1) {
          return;
        }
        DynamicJsonBuffer jsonBuffer;
        String input = "{\"txt\":\"tes\",\"kecepatan\":50,\"font\":50,\"animasi\":1,\"repeat\":2,\"brightness\":2}";
        JsonObject& root = jsonBuffer.parseObject(input);
        if (!root.success()) {
          return;
        };
        if (tex_pos > 1) {
          tex_pos = 0;
        };
        if (saatnya > 0) {
          char tx[40];
          switch (iqomah_pos) {
            case 1:
              sprintf(tx, "Saatnya menunaikan SHOLAT SUBUH");
              break;
            case 2:
              if (hari == 5) {
                sprintf(tx, "Saatnya menunaikan SHOLAT JUM'AT");
              } else {
                sprintf(tx, "Saatnya menunaikan SHOLAT DHUHUR");
              };
              break;
            case 3:
              sprintf(tx, "Saatnya menunaikan SHOLAT ASHAR");
              break;
            case 4:
              sprintf(tx, "Saatnya menunaikan SHOLAT MAGHRIB");
              break;
            case 5:
              sprintf(tx, "Saatnya menunaikan SHOLAT ISYA'");
              break;
            default:
              break;
          }
          root["txt"] = tx;
          root["kecepatan"] = kecepatan;
          root["brightness"] = brightness;
          root["animasi"] = 1;
          root["repeat"] = 1;
          root["font"] = 1;
          root.printTo(Serial);
          Serial.println();
          if (saatnya > 0) {
            saatnya--;
          };
        } else {
          if (hari == 5 && iqomah_pos == 2) {
            int nh = 0;
            while (on_r_txt_iqm[tex_pos + 2] == 0) {
              if (tex_pos < 1) {
                tex_pos++;
              }
              if (tex_pos > 1) {
                tex_pos = 0;
              };
              nh++;
              if (nh > 4) {
                return;
              };
            }
            root["txt"] = r_txt_iqm[tex_pos + 2];
            root["kecepatan"] = kecepatan;
            root["brightness"] = brightness;
            root["animasi"] = 1;
            root["repeat"] = 1;
            root["font"] = font_r_txt1_iqm[tex_pos + 2];
            root.printTo(Serial);
            Serial.println();
            tex_pos++;
            if (tex_pos > 1) {
              tex_pos = 0;
            };
          } else {
            int nh = 0;
            while (on_r_txt_iqm[tex_pos] == 0) {
              if (tex_pos < 1) {
                tex_pos++;
              }
              if (tex_pos > 1) {
                tex_pos = 0;
              };
              nh++;
              if (nh > 4) {
                return;
              };
            }
            root["txt"] = r_txt_iqm[tex_pos];
            root["kecepatan"] = kecepatan;
            root["brightness"] = brightness;
            root["animasi"] = 1;
            root["repeat"] = 1;
            root["font"] = font_r_txt1_iqm[tex_pos];
            root.printTo(Serial);
            Serial.println();
            tex_pos++;
            if (tex_pos > 1) {
              tex_pos = 0;
            };
          }
        }

      } else {
        if (on_off == 1) {
          return;
        };
        DynamicJsonBuffer jsonBuffer;
        String input = "{\"txt\":\"tes\",\"kecepatan\":50,\"font\":50,\"animasi\":1,\"repeat\":2,\"brightness\":2}";
        JsonObject& root = jsonBuffer.parseObject(input);
        if (!root.success()) {
          return;
        };
        int nh = 0;
        while (on_r_txt[tex_pos] == 0) {
          if (tex_pos < 4) {
            tex_pos++;
          }
          if (tex_pos > 4) {
            tex_pos = 0;
          };
          nh++;
          if (nh > 7) {
            return;
          };
        }
        //String txxt;
        //for (int a = 0; a < maks_txt[tex_pos]; a++) {
        //  txxt[a] = r_txt[tex_pos][a];
        //}
        root["txt"] = r_txt[tex_pos];
        root["kecepatan"] = kecepatan;
        root["brightness"] = brightness;
        root["animasi"] = 1;
        root["repeat"] = 1;
        root["font"] = font_r_txt1[tex_pos];
        root.printTo(Serial);
        Serial.println();
        tex_pos++;
        if (tex_pos > 4) {
          tex_pos = 0;
        };
      }
    }
  }
}

long millis_cek = millis();
void handle_cek_jadwal() {
  if ((millis() - millis_cek) < 2000) {
    return;
  } else {
    millis_cek = millis();
  }
  jadwal();
  if (on_off == 0) {
    digitalWrite(relay, HIGH);
  }
}

long millis_dip = millis();
long millis_tanggal = millis();
boolean dipping = false;
boolean dipping1 = false;
void handle_rutin() {
  if ((millis() - millis_tanggal) > 3000) {
    millis_tanggal = millis();
    if (dipping1 == false) {
      dipping1 = true;
      shari = 0;
    } else {
      dipping1 = false;
      shari = 1;
      jadwal();
      if (on_off == 1) {
        digitalWrite(relay, LOW);
      }
    }
    tampil();
  }

  if ((millis() - millis_dip) > 500) {
    millis_dip = millis();
    if (dipping == false) {
      dipping = true;
      dip = 1;
    } else {
      dipping = false;
      dip = 0;
    }
    tampil();
  }


}
