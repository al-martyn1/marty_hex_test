@set UMBA_MDPP_EXTRA_OPTIONS="--batch-generate-page-index" "--batch-page-index-file=doc/Index.md"

@set BRIEF_SCAN_PATHS="--scan=%~dp0"
@rem "--scan=%~dp0\../src" "--scan=%~dp0\../_src" "--scan=%~dp0\../examples"
@rem set BRIEF_EXTRA_OPTS_TXT=--scan-notes "--notes-output-path=%~dp0\doc\_md"
@set BRIEF_EXTRA_OPTS_MD=--scan-notes "--notes-output-path=%~dp0\doc\_md"

@call "%~dp0.bat\gen-doc.bat"
