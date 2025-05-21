

struct alsa_cb {
  snd_mixer_selem_id_t *sid;
  snd_mixer_t *handle;
  snd_mixer_elem_t *elem;
  long volume, minvol, maxvol;
};

void alsa_cpy(char *s, long vol) {
  static char icon[] = {0xf0, 0x9f, 0x94, 0x88};
  memcpy(s, icon, 4);
  s += 5;
  s[2] = ' ';
  memcpy(s, itos + vol * 2, 2 + (vol > 99));
}

void alsa(void *p, char *s) {
  static char icon[] = {0xf0, 0x9f, 0x94, 0x88};
  struct alsa_cb *cb = p;
  long vol;

  snd_mixer_handle_events(cb->handle);
  snd_mixer_selem_get_playback_volume(cb->elem, 0, &cb->volume);
  vol = cb->volume * 100 / (cb->maxvol - cb->minvol);
  alsa_cpy(s, vol);
}

int alsa_click(void *p, char *s, int btn) {
  struct alsa_cb *cb = p;
  int inc[2] = {3, -3};
  long vol;

  cb->volume += inc[btn];
  cb->volume = MAX(cb->volume, cb->minvol);
  cb->volume = MIN(cb->volume, cb->maxvol);
  snd_mixer_selem_set_playback_volume_all(cb->elem, cb->volume);
  vol = cb->volume * 100 / (cb->maxvol - cb->minvol);
  alsa_cpy(s, vol);

  return 1;
}

void *alsa_init() {
  struct alsa_cb *cb = malloc(sizeof(*cb));
  const char *card = "default";
  const char *selem_name = "Master";

  snd_mixer_open(&cb->handle, 0);
  snd_mixer_attach(cb->handle, card);
  snd_mixer_selem_register(cb->handle, NULL, NULL);
  snd_mixer_load(cb->handle);

  snd_mixer_selem_id_malloc(&cb->sid);
  snd_mixer_selem_id_set_index(cb->sid, 0);
  snd_mixer_selem_id_set_name(cb->sid, selem_name);
  cb->elem = snd_mixer_find_selem(cb->handle, cb->sid);
  snd_mixer_selem_get_playback_volume_range(cb->elem, &cb->minvol, &cb->maxvol);
  return cb;
}

void alsa_close(void *p) {
  struct alsa_cb *cb = p;
  snd_mixer_selem_id_free(cb->sid);
  snd_mixer_close(cb->handle);
  free(cb);
}
