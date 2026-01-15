# Appendix A: Adaptor Default Parameters

The Oracle and Sybase adaptors recognize default parameter values that let a user fine-tune interactions with a server. This appendix lists and describes these parameters.

---

## Oracle Parameters

The Oracle parameters are owned by `OracleAdaptor`. To set a parameter's value, you enter the following in a Terminal window:

```
> dwrite OracleAdaptor defaultKey defaultValue
```

The parameters names and the values that they take are described in the following sections.

### Login Information

The following parameters supply information when you log into the Oracle server:

- **OracleLoginName** — The user's login name for Oracle
- **OraclePassword** — The user's password
- **OracleSQLNetSpec** — A colon-separated list of three items: A single character SQL*Net code, a host name, and a server name. Taken together, these values identify the database server that you want to use.

These values can also be defined in a model file. The model file settings override the default parameter settings.

If the values for these parameters aren't available from the defaults system or the model file, the Database Kit uses `T` as the SQL*Net code and prompts the user for the host and server names (through the adaptor's login panel). See the Oracle documentation for more information on SQL*Net.

### UNIX Environment Variables

The Oracle server expects certain variables to be set in the UNIX environment. Values for these environment variables can be set through system parameters:

- **ORACLE_SID** — A system identifier. It defaults to `oracle`.
- **ORACLE_HOME** — Names the directory in which the server software is located. The default is `/usr/oracle`.
- **TWO_TASK** — Identifies the system for Oracle's "two-task" interface. It defaults to `MP:`.

For more information on the environment variables for which these system parameters are (exactly) named, see the Oracle documentation.

### Error Logging

The value of the **OracleLogErrors** parameter determines whether errors that emanate from the server are reported in the system console. By default, `OracleLogErrors` is `NO` (errors aren't reported in the console). If you change this to `YES`, a string will be printed to the console for each error the server generates or detects.

### Record Batch Size

When you fetch into a DBRecordList, batches of records are selected and copied into your application. The size of a single batch is, by default, algorithmically determined for a "best fit." Alternatively, you can force the size of the record batch by setting the value of the **OracleRecordBatchSize** parameter. The maximum number of records that can be batched is 2500.

> **Important:** If the records that you're fetching contain object values, the `OracleRecordBatchSize` parameter is ignored; the Database Kit sets the batch size automatically.

### Forced Outer Joins

The **OracleForceOuterJoin** parameter, if `YES`, forces all joins to be outer joins: If an equijoin is requested, the equijoin is automatically changed to an outer join. The value of `OracleForceOuterJoin` is, by default, `YES`. You can turn off this feature, thereby allowing equijoins, by setting the parameter to `NO`.

### Table Owners

When it asks for a list of entities from the Oracle adaptor, DBModeler only gets those that are owned by the user that has logged into the Oracle server. If you want to look at other users' entities, you must set the **OracleTableOwners** parameter. The parameter's value is a double-quoted list of Oracle users names. Each name in the list is in single-quotes and separated from the following name by a comma. For example:

```
> dwrite OracleAdaptor OracleTableOwners "'SCOTT','JOE','TONY'"
```

---

## Sybase Parameters

The Sybase parameters are owned by `SybaseAdaptor`. To set a parameter's value, you type the following in a Terminal window:

```
> dwrite SybaseAdaptor defaultKey defaultValue
```

The parameters names and the values that they take are described in the following sections.

### Login Information

The following parameters supply information when you log into the Sybase server:

- **SybaseLoginName** — The user's login name for Sybase. If none is supplied, the user's UNIX login name is used.
- **SybasePassword** — The user's password.
- **SybaseServer** — The name of the Sybase server. By default, it's set to `SYBASE`.
- **SybaseDatabase** — The name of the Sybase database.

These values can also be defined in a model file. The model file settings override the default parameter settings.

### The Interfaces File

The **SybaseInterfacesFile** parameter is a pathname that points to the location of the Sybase interfaces file. If the parameter isn't set, the file (in other words, a file named `interfaces`) is searched for in the adaptor's bundle. Failing that, the directory named by the `SYBASE` environment variable is searched.

### Logging

The **SybaseLogErrors** parameter takes a boolean value that determines whether errors that are detected by the server are described in messages sent to the console. The default is `NO`.

The **SybaseLogMessages** parameter does the same for general messages; the default for this parameter is also `NO`.

### Table Locking

When you send data back to the server, the table to which the data is written is first locked if the **SybaseLockTableOnUpdate** parameter is `YES` (this is the default). The lock is released after the update has finished. This is done by adding the `HOLDLOCK` keyword to the select that fetches the row to be modified. See the Sybase documentation for more information on table locking.

### Text Size

The **SybaseMaxTextsize** parameter is an integer that declares the largest size, in bytes, of a single datum. The default is 2147483647.

### Timeouts

The **SybaseLoginTimeout** is an integer that sets the period of time, in seconds, that the adaptor will wait for the server to respond while logging in. The default is 60 seconds. Setting the parameter to 0 causes the adaptor to wait forever.

The **SybaseTimeout** parameter does the same for operations other than logging in. For example, if you try to update data while the target table is locked, the adaptor returns control to your application if the server doesn't respond within `SybaseTimeout` seconds. The default is 0 (forever).

### Connections

The Sybase adaptor sets up one connection for fetching (or selecting) data, and another for updating data. The parameters **SybaseHoldsSelectConnection** and **SybaseHoldsUpdateConnection** take boolean values that determine whether these connections are established and held the entire time that your application is running (`YES`), or if they're created to perform their designated tasks and then killed when the operation is complete (`NO`). The default for `SybaseHoldsSelectConnection` is `YES`; that for `SybaseHoldsUpdateConnection` is `NO`.

---

*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*