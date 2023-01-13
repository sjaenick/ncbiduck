
WITH RECURSIVE hierarchy AS (
  SELECT taxid, parent_id FROM taxdata WHERE taxid=2675216
  UNION ALL
  SELECT t.taxid, t.parent_id
  FROM taxdata t, hierarchy h
  WHERE t.taxid=h.parent_id
)
SELECT h.taxid, h.parent_id, t.name, t.rank FROM hierarchy h
  LEFT JOIN taxdata t on h.taxid=t.taxid;


slower:

WITH RECURSIVE hierarchy AS (
  SELECT taxid, parent_id, name, rank FROM taxdata WHERE taxid=2675216
  UNION ALL
  SELECT t.taxid, t.parent_id, t.name, t.rank
  FROM taxdata t, hierarchy h
  WHERE t.taxid=h.parent_id
)
SELECT h.taxid, h.parent_id, h.name, h.rank FROM hierarchy h;



CREATE UNIQUE INDEX id_idx ON taxdata(taxid, parent_id);


