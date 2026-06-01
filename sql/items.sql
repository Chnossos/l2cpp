CREATE TABLE IF NOT EXISTS items (
    id            INTEGER PRIMARY KEY
  , template_id   INTEGER NOT NULL
  , owner_id      INTEGER          -- items on the ground have no owner
  , storage_id    INTEGER          -- conversely, can't have a storage id either
  , quantity      INTEGER          -- no value means item is not stackable
  , equipped      INTEGER          -- no value means item can't be equipped
  , enchant_level INTEGER          -- no value means item can't be enchanted
  , FOREIGN KEY (template_id) REFERENCES item_templates (id) ON DELETE CASCADE
  , FOREIGN KEY (owner_id)    REFERENCES characters     (id) ON DELETE CASCADE
);
