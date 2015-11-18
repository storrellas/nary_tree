DROP TABLE IF EXISTS employee;
DROP TABLE IF EXISTS employee_stats;
-- Table: employee
CREATE TABLE employee ( 
    id         INTEGER        PRIMARY KEY AUTOINCREMENT,
    supervisor INT            REFERENCES employee ( id ),
    name       VARCHAR( 20 ) 
);

INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (1, null, 'Sergi');
INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (2, 1, 'Albert');
INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (3, 2, 'Joan');
INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (4, 1, 'Carles');
INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (5, 3, 'Josep');
INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (6, 4, 'Maria');
INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (7, 4, 'Anna');

-- Table: employee_stats
CREATE TABLE employee_stats ( 
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    employee_id INTEGER REFERENCES employee,
    n_sup       INTEGER,
    n_t_sup     INTEGER,
    depth       INTEGER 
);

