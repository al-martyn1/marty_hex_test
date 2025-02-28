@rem Ограничение строки командной строки (Cmd. exe) - https://learn.microsoft.com/ru-ru/troubleshoot/windows-client/shell-experience/command-line-string-limitation
@for %%I in ("%~dp0\*test-*.bat") do @(
    @echo Calling BAT: "%%I"
    @call "%%I"
)
