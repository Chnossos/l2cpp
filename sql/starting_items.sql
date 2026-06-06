CREATE TABLE IF NOT EXISTS starting_items (
    profession       INTEGER          -- NULL means item is given regardless of profession
  , item_template_id INTEGER NOT NULL
  , quantity         INTEGER          -- NULL means item is not stackable
  , equipped         BOOLEAN          -- NULL means item is not equippable
  , FOREIGN KEY (profession)       REFERENCES professions    (id) ON DELETE CASCADE
  , FOREIGN KEY (item_template_id) REFERENCES item_templates (id) ON DELETE CASCADE
);

CREATE TRIGGER IF NOT EXISTS starting_items_trigger
    AFTER
        INSERT ON characters
    BEGIN
        INSERT INTO
            items (template_id, owner_id, storage_id, quantity, equipped)
        SELECT
            item_template_id, NEW.id, 0, quantity, equipped
        FROM
            starting_items
        WHERE
            profession IS NULL OR profession = NEW.starting_profession;
    END;
