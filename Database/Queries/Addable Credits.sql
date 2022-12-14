USE [atum2_db_1]
GO
/****** Object:  StoredProcedure [dbo].[Billing_Get_Point]    Script Date: 04.08.2015 01:00:02 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

-- =============================================
-- Author:		Future
-- Create date: 04 08 2015
-- Description:	Sets the amount of credits on an account
-- =============================================
CREATE PROCEDURE [dbo].[Billing_Add_Points]
	@i_AccID			INT,
	@i_PointsToAdd		INT
AS
	SET NOCOUNT ON;

	UPDATE TOP(1) atum2_db_account.dbo.td_Account SET CashPoint = CashPoint + @i_PointsToAdd WHERE AccountUniqueNumber = @i_AccID

	SELECT CashPoint FROM atum2_db_account.dbo.td_Account WHERE AccountUniqueNumber = @i_AccID

