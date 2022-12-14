USE [atum2_db_1]
GO
/****** Object:  StoredProcedure [dbo].[atum_StoreUpdateColorCode]    Script Date: 10.06.2015 00:45:28 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--====
-- Name:
-- Desc:2015-06-10 by St0rmy
--		Simply update the colorcode
--====
ALTER PROCEDURE [dbo].[atum_StoreUpdateColorCode]
	@UniqueNumber	BIGINT,
	@ColorCode		INT
AS
	UPDATE td_Store
	SET ColorCode = @ColorCode
	WHERE UniqueNumber = @UniqueNumber
